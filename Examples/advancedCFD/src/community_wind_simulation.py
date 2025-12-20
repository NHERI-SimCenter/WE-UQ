from __future__ import annotations
import argparse
from pathlib import Path
import sys, math
import numpy as np
import pandas as pd
import geopandas as gpd
import shapely.geometry as geom
import shapely.affinity as affinity
import trimesh
import textwrap
import re
import json
from brails.utils import Importer
from pyproj import Geod
from pyproj import CRS
from typing import List, Tuple

# ───────────────────────── interactive input ─────────────────────────
def ask_float(prompt: str, default: str | None = None) -> float:
    while True:
        txt = input(f"{prompt}{f' [{default}]' if default else ''}: ").strip()
        if txt == "" and default is not None:
            txt = default
        try:
            return float(txt)
        except ValueError:
            print("  ↳ please enter a valid number")

def ask_mult(prompt: str, default_mult: float) -> float:
    """Return a **multiplier** (not yet multiplied by Hmax)."""
    s = input(f"{prompt}  (Default = {default_mult}) ").strip()
    if s == "":
        return default_mult
    try:
        value = float(s)
        if value < 0:
            raise ValueError
        return value
    except ValueError:
        print("  ↳ please enter a non-negative number")
        return ask_float(prompt, default_mult)

def ask_path(prompt: str, default: str) -> Path:
    name = input(f"{prompt} [{default}]: ").strip() or default
    return Path(name)

def ask_file(prompt: str) -> Path:
    while True:
        p = Path(input(f"{prompt}: ").strip())
        if p.exists():
            return p
        print("  ↳ file not found, try again.")

def ask_dir(prompt: str, default="case") -> Path:
    p = Path(input(f"{prompt} [{default}]: ").strip() or default)
    p.mkdir(parents=True, exist_ok=True)
    return p

def ask_side() -> str:
    val = input("Side boundary type {cyclic|slip} [cyclic]: ").strip().lower() or "cyclic"
    while val not in ("cyclic", "slip"):
        val = input("  ↳ choose cyclic or slip: ").strip().lower() or "cyclic"
    return val

def ask_angle() -> float:
    while True:
        try:
            deg = float(input("Wind-direction angle θ (deg, Counter-Clockwise from E): ").strip())
            return deg
        except ValueError:
            print("  ↳ enter a numeric value")

def ask_choice(prompt: str, choices: list[str], default: str) -> str:
    txt = input(f"{prompt} [{default}]: ").strip().upper() or default
    while txt not in choices:
        txt = input(f"  ↳ choose one of {choices}: ").strip().upper() or default
    return txt

def ask_box(idx: int) -> tuple | None:
    print(f"\n--- Box #{idx}  (ENTER empty x-min to finish) ---")
    x_min = input("  x-min: ").strip()
    if x_min == "":
        return None

    coords = [x_min]
    for lbl in ("y-min", "z-min", "x-max", "y-max", "z-max"):
        coords.append(input(f"  {lbl}: ").strip())

    try:
        vals = tuple(float(v) for v in coords)
    except ValueError:
        print("  ✗  invalid number — try again")
        return ask_box(idx)

    default_name = f"box{idx}"
    name = input(f"  name for this box [{default_name}]: ").strip() or default_name

    # ask for a single integer level
    while True:
        lev_raw = input("  level (e.g. 3): ").strip()
        if lev_raw.isdigit():
            lev = lev_raw               # keep as string
            break
        print("  ↳ enter an integer value")

    return name, vals, lev    # tuple with 3 elements

def ask(prompt: str, default: str) -> str:
    ans = input(f"{prompt} [{default}]: ").strip()
    return ans or default

def get_input(prompt: str, default: str) -> str:
    s = input(f"{prompt} [{default}]: ").strip()
    return s or default

def ask_out(msg: str, default: str) -> Path:
    return Path(input(f"{msg} [{default}]: ").strip() or default)


# ───────────────────────── Make domain bounds ─────────────────────────
def make_bounds(xmin, xmax, ymin, ymax, hmax, inmult, outmult, sidmult, inlet: str):
    """
    Return x0, x1, y0, y1 with padding rules:

        inlet   : 5 * Hmax upstream
        outlet  : 15 * Hmax downstream
        lateral : ±5 * Hmax
        top     : handled elsewhere (always 15 * Hmax)
    """
    if inlet == "W":            # flow +x
        x0, x1 = xmin - inmult*hmax, xmax + outmult*hmax
        y0, y1 = ymin - sidmult*hmax, ymax + sidmult*hmax
    elif inlet == "E":          # flow –x
        x0, x1 = xmin - outmult*hmax, xmax + inmult*hmax
        y0, y1 = ymin - sidmult*hmax, ymax + sidmult*hmax
    elif inlet == "S":          # flow +y
        x0, x1 = xmin - sidmult*hmax, xmax + sidmult*hmax
        y0, y1 = ymin - inmult*hmax, ymax + outmult*hmax
    else:                       # inlet == "N"  (flow –y)
        x0, x1 = xmin - sidmult*hmax, xmax + sidmult*hmax
        y0, y1 = ymin - outmult*hmax, ymax + inmult*hmax
    return x0, x1, y0, y1

def utm_epsg(lon: float, lat: float) -> int:
    z = int((lon + 180)//6)+1
    return 32600+z if lat >= 0 else 32700+z

def to_metric(gdf: gpd.GeoDataFrame, epsg:int):
    if gdf.crs is None:
        gdf = gdf.set_crs(4326)
    if CRS.from_user_input(gdf.crs).is_geographic:
        gdf = gdf.to_crs(epsg)
    return gdf

# ───────────────────────── Add the nearest height if data missing ─────────────────────────

def nearest_fill(gdf: gpd.GeoDataFrame, fld:str, dflt:float):
    kn = gdf[~gdf[fld].isna()].copy()
    if kn.empty:
        gdf[fld] = dflt
        return gdf
    sidx = kn.sindex
    for idx, g in gdf[gdf[fld].isna()].geometry.items():
        near = sidx.nearest(g, return_all=False)
        pos  = int(near[0]) if isinstance(near, np.ndarray) else int(next(near))
        gdf.at[idx, fld] = kn.iloc[pos][fld]
    return gdf

# ───────── Write STL Files ────────────────────────────────────────────
def extrude(poly: geom.base.BaseGeometry, h: float):
    return trimesh.creation.extrude_polygon(poly, height=h)

def write_stl_ascii(mesh: trimesh.Trimesh, path: Path, name="mesh"):
    if mesh.faces.shape[1] != 3:
        mesh = mesh.triangulate()
    v, f = mesh.vertices, mesh.faces
    def nrm(t): n = np.cross(t[1]-t[0], t[2]-t[0]); l = np.linalg.norm(n); return n/l if l else n
    with path.open("w") as fh:
        fh.write(f"solid {name}\n")
        for a,b,c in f:
            tri = v[[a,b,c]]; nx,ny,nz = nrm(tri)
            fh.write(f"  facet normal {nx:.6e} {ny:.6e} {nz:.6e}\n    outer loop\n")
            for x,y,z in tri:
                fh.write(f"      vertex {x:.6e} {y:.6e} {z:.6e}\n")
            fh.write("    endloop\n  endfacet\n")
        fh.write(f"endsolid {name}\n")

# ───────── Write BlockMeshDict ───────────────────────────────────
def write_bmd(x0,x1,y0,y1,z0,z1,nx,ny,nz,side_type,out:Path):
    out.parent.mkdir(parents=True, exist_ok=True)
    if side_type == "cyclic":
        side1 = "side1  { type cyclic; neighbourPatch side2; faces ((0 1 5 4)); }"
        side2 = "side2  { type cyclic; neighbourPatch side1; faces ((2 3 7 6)); }"
    else:
        side1 = "side1  { type patch; faces ((0 1 5 4)); }"
        side2 = "side2  { type patch; faces ((2 3 7 6)); }"

    txt=f"""
    /*--------------------------------*- C++ -*----------------------------------*\\
    | =========                 |                                                 |
    | \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
    |  \\    /   O peration     | Version:  10                                    |
    |   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
    |    \\/     M anipulation  |                                                 |
    \\*---------------------------------------------------------------------------*/
    FoamFile{{\n version 2.0;\n format ascii;\n class dictionary;\n object blockMeshDict;}}
    convertToMeters 1.0;

    vertices
    (
     ({x0} {y0} {z0}) ({x1} {y0} {z0}) ({x1} {y1} {z0}) ({x0} {y1} {z0})
     ({x0} {y0} {z1}) ({x1} {y0} {z1}) ({x1} {y1} {z1}) ({x0} {y1} {z1})
    );

    blocks
    (
     hex (0 1 2 3 4 5 6 7) ({nx} {ny} {nz}) simpleGrading (1 1 1)
    );

    boundary
    (
     inlet   {{ type patch; faces ((0 4 7 3)); }}
     outlet  {{ type patch; faces ((1 2 6 5)); }}
     {side1}
     {side2}
     top     {{ type patch; faces ((4 5 6 7)); }}
     ground  {{ type wall;  faces ((0 3 2 1)); }}
    );

    mergePatchPairs();
    """
    out.write_text(txt)



# ───────────────────────── Max refinement level of boxes ─────────────────────────
def max_numeric_level(level_strings: List[str]) -> int:
    nums: List[int] = []
    for s in level_strings:
        nums += list(map(int, re.findall(r"\d+", s)))
    return max(nums) if nums else 1

# ───────────────────────── Templates for various files ─────────────────────────
TEMPLATE = textwrap.dedent("""
/*--------------------------------*- C++ -*----------------------------------*\\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  10                                 |
|   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
|    \\/     M anipulation  |                                                 |
\\*---------------------------------------------------------------------------*/
FoamFile {{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "system";
    object      snappyHexMeshDict;
}}


castellatedMesh on;
snap            on;
addLayers       off;

geometry
{{
    ROI
    {{
        type triSurfaceMesh;
        file "{roi_stl}.stl";
    }}
    Surrounding
    {{
        type triSurfaceMesh;
        file "{sur_stl}.stl";
    }}
{geometry_entries}
}};

castellatedMeshControls
{{
    maxLocalCells 100000000;
    maxGlobalCells 2000000000;
    minRefinementCells 10;
    maxLoadUnbalance 0.10;
    nCellsBetweenLevels {ncellblvl};
    resolveFeatureAngle 30;

    features
    (
        {{ file "{roi_stl}.eMesh";          level {roi_feat_level}; }}
        {{ file "{sur_stl}.eMesh";  level {sur_feat_level}; }}
    );

    refinementSurfaces
    {{
        ROI
        {{
            level ({roi_level} {roi_level});
            patchInfo {{ type wall; }}
        }}
        Surrounding
        {{
            level ({sur_level} {sur_level});
            patchInfo {{ type wall; }}
        }}
    }}

    refinementRegions
    {{
{refinement_entries}
    }}
    insidePoint (0.01 0.01 0.01);
    allowFreeStandingZoneFaces false;
}}

snapControls
{{
    nSmoothPatch 3;
    tolerance 2.0;
    nSolveIter 30;
    nRelaxIter 5;
    nFeatureSnapIter 10;
    implicitFeatureSnap false;
    explicitFeatureSnap true;
    multiRegionFeatureSnap false;
}}

meshQualityControls
{{
     maxNonOrtho 65;

    //- Max skewness allowed. Set to <0 to disable.
    maxBoundarySkewness 20;
    
    maxInternalSkewness 4;

    //- Max concaveness allowed. Is angle (in degrees) below which concavity
    //  is allowed. 0 is straight face, <0 would be convex face.
    //  Set to 180 to disable.
    maxConcave 80;

    //- Minimum projected area v.s. actual area. Set to -1 to disable.
    minFlatness 0.5;

    //- Minimum cell pyramid volume relative to min bounding box length^3
    //  Set to a fraction of the smallest cell volume expected.
    //  Set to very negative number (e.g. -1e30) to disable.
    minVol -1e30;

    minTetQuality 1e-30;

    //- Minimum face twist. Set to <-1 to disable. dot product of face normal
    //  and face centre triangles normal
    minTwist 0.02;

    //- Minimum normalised cell determinant
    //  1 = hex, <= 0 = folded or flattened illegal cell
    minDeterminant 0.001;

    //- minFaceWeight (0 -> 0.5)
    minFaceWeight 0.02;

    //- minVolRatio (0 -> 1)
    minVolRatio 0.01;

    // Advanced

    //- Number of error distribution iterations
    nSmoothScale 4;
    //- Amount to scale back displacement at error points
    errorReduction 0.75;
    
    minArea  0;

    minTriangleTwist  0;
}}
debug 0;
mergeTolerance 1e-6;
""")

GEOM_BOX = "    {name}\n    {{\n        type searchableBox;\n" \
           "        min ({xmin} {ymin} {zmin});\n" \
           "        max ({xmax} {ymax} {zmax});\n    }}"

REFINE_BOX = "        {name}\n        {{\n            mode inside;\n" \
             "            level {level};\n        }}"

def cli() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="Create turbulenceProperties and initial fields "
                    "(RANS k-ε or LES with mean ABL or turbulent DFM inlet).")
    
    return p.parse_args()

TP_RANS = textwrap.dedent("""
/*--------------------------------*- C++ -*----------------------------------*\\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  10                                 |
|   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
|    \\/     M anipulation  |                                                 |
\\*---------------------------------------------------------------------------*/
FoamFile{
    version 2.0;
    format ascii;
    class dictionary;
    object turbulenceProperties;
}

simulationType RAS;

RAS
{
    RASModel kEpsilon;
    turbulence on;
    printCoeffs on;
}
""")

TP_LES = textwrap.dedent("""
/*--------------------------------*- C++ -*----------------------------------*\\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  10                                 |
|   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
|    \\/     M anipulation  |                                                 |
\\*---------------------------------------------------------------------------*/
FoamFile{
    version 2.0;
    format ascii;
    class dictionary;
    object turbulenceProperties;
}

simulationType LES;

LES
{
    LESModel Smagorinsky;
    delta cubeRootVol;
    turbulence on;
    printCoeffs on;

    SmagorinskyCoeffs
    {
        Cs 0.17;
    }
}
""")

# ───────────── field file header ─────────────
FIELD_HEADER = """
"""

# ──────────── Templates for various fields ────────────
def body_U_meanABL(side, Uref, Href, z0):
    return textwrap.dedent(f"""
        /*--------------------------------*- C++ -*----------------------------------*\\
        | =========                 |                                                 |
        | \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
        |  \\    /   O peration     | Version:  10                                 |
        |   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
        |    \\/     M anipulation  |                                                 |
        \\*---------------------------------------------------------------------------*/
        FoamFile
        {{
            version     2.0;
            format      ascii;
            class       volVectorField;
            location    "0";
            object      U;
        }}
        dimensions      [0 1 -1 0 0 0 0];
        internalField   uniform (0 0 0);
        boundaryField
        {{
            inlet
            {{
                type atmBoundaryLayerInletVelocity;
                Uref {Uref};
                Zref {Href};
                zDir (0 0 1);
                flowDir (1 0 0);
                z0 uniform {z0};
                zGround uniform 0;
                value uniform ({Uref} 0 0);
            }}
            outlet {{ type pressureInletOutletVelocity; value uniform (0 0 0); }}
            side1  {{ type {side}; }}
            side2  {{ type {side}; }}
            top    {{ type slip; }}
            ground        {{ type fixedValue; value uniform (0 0 0); }}
            ROI           {{ type fixedValue; value uniform (0 0 0); }}
            Surrounding   {{ type fixedValue; value uniform (0 0 0); }}
        }}
    """)

def body_U_DFM(side, Uref):
    return textwrap.dedent(f"""
        /*--------------------------------*- C++ -*----------------------------------*\\
        | =========                 |                                                 |
        | \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
        |  \\    /   O peration     | Version:  10                                 |
        |   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
        |    \\/     M anipulation  |                                                 |
        \\*---------------------------------------------------------------------------*/
        FoamFile
        {{
            version     2.0;
            format      ascii;
            class       volVectorField;
            location    "0";
            object      U;
        }}
        dimensions      [0 1 -1 0 0 0 0];
        internalField   uniform (0 0 0);
        boundaryField
        {{
            inlet
            {{
                type turbulentDFMInlet;
                filterType  exponential;
                filterFactor 4;
                value uniform ( {Uref} 0 0 );
                periodicInY true;
                periodicInZ false;
                constMeanU true;
                Uref {Uref};
            }}
            outlet {{ type pressureInletOutletVelocity; value uniform (0 0 0); }}
            side1  {{ type {side}; }}
            side2  {{ type {side}; }}
            top    {{ type slip; }}
            ground        {{ type fixedValue; value uniform (0 0 0); }}
            ROI           {{ type fixedValue; value uniform (0 0 0); }}
            Surrounding   {{ type fixedValue; value uniform (0 0 0); }}
        }}
    """)

def body_k(side, Uref, Href, z0):
    return textwrap.dedent(f"""
        /*--------------------------------*- C++ -*----------------------------------*\\
        | =========                 |                                                 |
        | \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
        |  \\    /   O peration     | Version:  10                                 |
        |   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
        |    \\/     M anipulation  |                                                 |
        \\*---------------------------------------------------------------------------*/
        FoamFile
        {{
            version     2.0;
            format      ascii;
            class       volScalarField;
            location    "0";
            object      k;
        }}
        dimensions      [0 2 -2 0 0 0 0];
        internalField   uniform 0.06;
        boundaryField
        {{
            inlet
            {{
                type atmBoundaryLayerInletK;
                Uref {Uref};
                Zref {Href};
                zDir (0 0 1);
                flowDir (1 0 0);
                z0 uniform {z0};
                zGround uniform 0;
                value uniform 0.06;
            }}
            outlet {{ type zeroGradient; }}
            side1 {{ type {side}; }}
            side2 {{ type {side}; }}
            top   {{ type slip; }}
            ground        {{ type kqRWallFunction; value $internalField; }}
            ROI           {{ type kqRWallFunction; value $internalField; }}
            Surrounding   {{ type kqRWallFunction; value $internalField; }}
        }}
    """)

def body_eps(side, Uref, Href, z0):
    return textwrap.dedent(f"""
        /*--------------------------------*- C++ -*----------------------------------*\\
        | =========                 |                                                 |
        | \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
        |  \\    /   O peration     | Version:  10                                 |
        |   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
        |    \\/     M anipulation  |                                                 |
        \\*---------------------------------------------------------------------------*/
        FoamFile
        {{
            version     2.0;
            format      ascii;
            class       volScalarField;
            location    "0";
            object      epsilon;
        }}
        dimensions      [0 2 -3 0 0 0 0];
        internalField   uniform 0.09;
        boundaryField
        {{
            inlet
            {{
                type atmBoundaryLayerInletEpsilon;
                Uref {Uref};
                Zref {Href};
                zDir (0 0 1);
                flowDir (1 0 0);
                z0 uniform {z0};
                zGround uniform 0;
                Cmu 0.09;
                kappa 0.41;
                value uniform 0.09;
            }}
            outlet {{ type zeroGradient; }}
            side1 {{ type {side}; }}
            side2 {{ type {side}; }}
            top   {{ type slip; }}
            ground        {{ type epsilonWallFunction; value $internalField; }}
            ROI           {{ type epsilonWallFunction; value $internalField; }}
            Surrounding   {{ type epsilonWallFunction; value $internalField; }}
        }}
    """)

def body_nut(side, ground_style, z0, Uref, Href, roi_style, surround_style):
    if ground_style == "rough":
        gf = f"type nutkAtmRoughWallFunction;\n        z0 uniform {z0};\n        value uniform 0;"
    else:
        gf = ("type nutkWallFunction;\n        Cmu 0.09;\n        kappa 0.42;\n"
              "        E 9.793;\n        value uniform 0;")
    

    if roi_style == "rough":
        rf = f"type nutkAtmRoughWallFunction;\n        z0 uniform {z0};\n        value uniform 0;"
    else:
        rf = ("type nutkWallFunction;\n        Cmu 0.09;\n        kappa 0.42;\n"
              "        E 9.793;\n        value uniform 0;")
    

    if surround_style == "rough":
        sf = f"type nutkAtmRoughWallFunction;\n        z0 uniform {z0};\n        value uniform 0;"
    else:
        sf = ("type nutkWallFunction;\n        Cmu 0.09;\n        kappa 0.42;\n"
              "        E 9.793;\n        value uniform 0;")
    

    return textwrap.dedent(f"""
        /*--------------------------------*- C++ -*----------------------------------*\\
        | =========                 |                                                 |
        | \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
        |  \\    /   O peration     | Version:  10                                 |
        |   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
        |    \\/     M anipulation  |                                                 |
        \\*---------------------------------------------------------------------------*/
        FoamFile
        {{
            version     2.0;
            format      ascii;
            class       volScalarField;
            location    "0";
            object      nut;
        }}
        dimensions      [0 2 -1 0 0 0 0];
        internalField   uniform 5e-05;
        boundaryField
        {{
            Uref {Uref};
            Zref {Href};
            zDir (0 0 1);
            flowDir (1 0 0);
            z0 uniform {z0};
            zGround uniform 0;
            Cmu 0.09;
            kappa 0.41;
            value uniform 0.09;
            inlet  {{ type calculated; value uniform 0; }}
            outlet {{ type calculated; value uniform 0; }}
            side1  {{ type {side}; }}
            side2  {{ type {side}; }}
            top    {{ type slip; }}
            ground        {{ {gf} }}
            ROI           {{ {rf} }}
            Surrounding   {{ {sf} }}
        }}
    """)

P_BODY = """
/*--------------------------------*- C++ -*----------------------------------*\\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  10                                 |
|   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
|    \\/     M anipulation  |                                                 |
\\*---------------------------------------------------------------------------*/
FoamFile
     {
         version     2.0;
         format      ascii;
         class       volScalarField;
         location    "0";
         object      p;
     }
dimensions [0 2 -2 0 0 0 0];
internalField uniform 0;
boundaryField
{
    inlet  { type zeroGradient; }
    outlet { type fixedValue; value uniform 0; }
    side1  { type %s; }
    side2  { type %s; }
    top    { type slip; }
    ground        { type zeroGradient; }
    ROI           { type zeroGradient; }
    Surrounding   { type zeroGradient; }
}
"""

SIDE_MAP = {"cyclic": "cyclic", "slip": "slip"}

# ───────────────────────── Templates for ControlDict ─────────────────────────

TP_RANSC = textwrap.dedent("""\
    /*--------------------------------*- C++ -*----------------------------------*\\
    | =========                 |                                                 |
    | \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
    |  \\    /   O peration     | Version:  10                                    |
    |   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
    |    \\/     M anipulation  |                                                 |
    \\*---------------------------------------------------------------------------*/
    FoamFile
    {{
        version 2.0;
        format  ascii;
        class   dictionary;
        location "system";
        object  controlDict;
    }}

    application  simpleFoam;

    startFrom latestTime;
                          
    startTime 0;
    
    stopAt endTime;
    
    endTime {end_T};  

    deltaT {deltaT_sim};
    
    writeControl timeStep;

    writeInterval {deltaT_write};
    
    purgeWrite 0;
    
    writeFormat ascii;

    writePrecision 6;
    
    writeCompression compressed;
    
    timeFormat general;

    timePrecision 6;

    runTimeModifiable true;

    libs ("libatmosphericModels.so");

    

""")

TP_LESC  = textwrap.dedent("""\
    /*--------------------------------*- C++ -*----------------------------------*\\
    | =========                 |                                                 |
    | \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
    |  \\    /   O peration     | Version:  10                                 |
    |   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
    |    \\/     M anipulation  |                                                 |
    \\*---------------------------------------------------------------------------*/
    FoamFile
    {{
        version 2.0;
        format  ascii;
        class   dictionary;
        location "system";
        object  controlDict;
    }}

    application  {les_algorithm};

    startFrom latestTime;
                          
    startTime 0.0;
    
    stopAt endTime;
    
    endTime {end_T};  

    deltaT {deltaT_sim};
    
    writeControl runTime;

    writeInterval {deltaT_write};
    
    purgeWrite 0;
    
    writeFormat binary;

    writePrecision 8;
    
    writeCompression off;
    
    timeFormat general;

    timePrecision 6;

    runTimeModifiable yes;
                          
    adjustTimeStep {adjust_time};

    maxCo {maxCo};

    maxDeltaT {maxDeltaT};

    libs ("libatmosphericModels.so" {lib_TINF});

    functions
    {{
    {functions}
    }}
                          
""")


TP_PROFILE  = textwrap.dedent("""\
    {ProfileName}
    {{
        type            probes;
        libs ("libsampling.so");
                                
        writeControl   timeStep;                          
        writeInterval  {N_profile};
        timeStart {t_start};
                                
        fields          ({fields});

        probeLocations
        (
            {points}
        );
    }}
                             
""")

TP_PLANE  = textwrap.dedent("""\
    {PlaneName}
    {{
        type            surfaces;
        libs ("libsampling.so");
                                
        writeControl   timeStep;                          
        writeInterval  {N_plane};
        timeStart {t_start};
        timeEnd {t_end};

        surfaceFormat   	vtk;                             
        fields          ({fields});
        
        interpolationScheme cellPoint;

        surfaces
        (
            sectionPlane
            {{
                type cuttingPlane;
                planeType pointAndNormal;
                point {plane_point};
                normal {plane_normal};
                interpolate true;
            }}
        );
    }}
                                
    
""")

# ───────────────────────── Templates for Transport Properties, fvSolution, fvSchemes, surfaceFeatures ─────────────────────────

TP_transportProperty = textwrap.dedent("""
/*--------------------------------*- C++ -*----------------------------------*\\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  10                                 |
|   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
|    \\/     M anipulation  |                                                 |
\\*---------------------------------------------------------------------------*/
FoamFile
{{
    version 2.0;
    format  ascii;
    class   dictionary;
    location "constant";
    object  transportProperties;
}}

transportModel  Newtonian;

nu              [0 2 -1 0 0 0 0] {nu};

""")


# mapping for les algorithms
LES_ALGORITHM_MAP = {
    "pisoFoam": "pisoFoam",
    "pimpleFoam":   "pimpleFoam"
}

FIELD_MAP = {
    "U": "U",
    "p": "p",
    "U p": "U p",
    "p U": "U p"
}

TP_surfFeat = textwrap.dedent("""
/*--------------------------------*- C++ -*----------------------------------*\\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  www.OpenFoam.org
    \\  /    A nd           | Version:  10
     \\/     M anipulation  |
\\*---------------------------------------------------------------------------*/
FoamFile
{
    format      ascii;
    class       dictionary;
    object      surfaceFeaturesDict;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

surfaces  ("Surrounding.stl" "ROI.stl");
#includeEtc "caseDicts/surface/surfaceFeaturesDict.cfg"

// ************************************************************************* //


""")

TP_fvSch_RANS = textwrap.dedent("""
/*--------------------------------*- C++ -*----------------------------------*\\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  www.OpenFoam.org
    \\  /    A nd           | Version:  10
     \\/     M anipulation  |
\\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "system";
    object      fvSchemes;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

ddtSchemes
{
    default         steadyState;
}

gradSchemes
{
    default         cellLimited Gauss linear 0.5;
}

divSchemes
{
    default         none;
    div(phi,U)      bounded Gauss linearUpwindV grad(U);
    div(phi,k)      bounded Gauss upwind; 
    div(phi,epsilon) bounded Gauss upwind;
    div((nuEff*dev2(T(grad(U))))) Gauss linear;
}

laplacianSchemes
{
    default         Gauss linear limited 0.5;
}

interpolationSchemes
{
    default         linear;
    interpolate(U)  linear;
}

snGradSchemes
{
    default         limited 0.5;
}

fluxRequired
{
    default         no;
    p               ;
}


// ************************************************************************* //


""")

TP_fvSol_RANS = textwrap.dedent("""
/*--------------------------------*- C++ -*----------------------------------*\\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  www.OpenFoam.org
    \\  /    A nd           | Version:  10
     \\/     M anipulation  |
\\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "system";
    object      fvSolution;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

solvers
{
    p                                  // linear equation system solver for p
    {
        solver          GAMG;           // very efficient multigrid solver
        tolerance       1e-09;          // solver finishes if either absolute
        relTol          0.001;          // tolerance is reached or the relative
                                        // tolerance here
        minIter         3;              // a minimum number of iterations
        maxIter         100;            // limitation of iterions number
        smoother        DIC;            // setting for GAMG
        nPreSweeps      1;              // 1 for p, set to 0 for all other!
        nPostSweeps     2;              // 2 is fine
        nFinestSweeps   2;              // 2 is fine
        scaleCorrection true;           // true is fine
        directSolveCoarsestLevel false; // false is fine
        cacheAgglomeration on;          // on is fine; set to off, if dynamic
                                        // mesh refinement is used!
        nCellsInCoarsestLevel 500;      // 500 is fine,
                                        // otherwise sqrt(number of cells)
        agglomerator    faceAreaPair;   // faceAreaPair is fine
        mergeLevels     1;              // 1 is fine
    }

    U                                   // linear equation system solver for U
    {
        solver          smoothSolver;   // solver type
        smoother        GaussSeidel;    // smoother type
        tolerance       1e-09;          // solver finishes if either absolute
        relTol          0.001;           // tolerance is reached or the relative
                                        // tolerance here
        nSweeps         1;              // setting for smoothSolver
        maxIter         100;            // limitation of iterations number
    }

    k
    {
        solver          smoothSolver;   // solver type
        smoother        GaussSeidel;    // smoother type
        tolerance       1e-09;          // solver finishes if either absolute
        relTol          0.001;           // tolerance is reached or the relative
                                        // tolerance here
        nSweeps         1;              // setting for smoothSolver
        maxIter         100;            // limitation of iterations number
    }

    epsilon
    {
        solver          smoothSolver;   // solver type
        smoother        GaussSeidel;    // smoother type
        tolerance       1e-09;          // solver finishes if either absolute
        relTol          0.001;           // tolerance is reached or the relative
                                        // tolerance here
        nSweeps         1;              // setting for smoothSolver
        maxIter         100;            // limitation of iterations number
    }

    R
    {
        solver          PBiCG;
        preconditioner  DILU;
        tolerance       1e-06;
//        relTol          0.1;
	relTol          0.001;
    }

    nuTilda
    {
        solver          PBiCG;
        preconditioner  DILU;
        tolerance       1e-06;
//        relTol          0.1;
	relTol          0.001;
    }
}

SIMPLE
{
    nNonOrthogonalCorrectors 0;

    residualControl
    {
        p               1e-6;
        U               1e-6;
        "(k|epsilon|omega)" 1e-6;
    }
}

potentialFlow
{
	nNonOrthogonalCorrectors 3;
}

relaxationFactors
{
    p               0.3;
    U               0.7;
    k               0.7;
    epsilon         0.7;
    R               0.7;
    nuTilda         0.7;
}

// ************************************************************************* //


""")

TP_fvSch_LES = textwrap.dedent("""
/*--------------------------------*- C++ -*----------------------------------*\\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  www.OpenFoam.org
    \\  /    A nd           | Version:  10
     \\/     M anipulation  |
\\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "system";
    object      fvSchemes;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

ddtSchemes
{
    default         backward;
}

gradSchemes
{
    default         Gauss linear;
}

divSchemes
{
    default         none;
    div(phi,U)      Gauss filteredLinear3V 1;
    div(phi,k)      Gauss limitedLinear 1;
    div(phi,B)      Gauss limitedLinear 1;
    div(B)          Gauss linear;
    div(phi,nuTilda) Gauss limitedLinear 1;
    div((nuEff*dev2(T(grad(U))))) Gauss linear;
}

laplacianSchemes
{
    default         Gauss linear corrected;
}

interpolationSchemes
{
    default         linear;
}

snGradSchemes
{
    default         corrected;
}

wallDist
{
    method meshWave;
}



// ************************************************************************* //


""")

TP_fvSol_LES = textwrap.dedent("""
/*--------------------------------*- C++ -*----------------------------------*\\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  www.OpenFoam.org
    \\  /    A nd           | Version:  10
     \\/     M anipulation  |
\\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "system";
    object      fvSolution;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

solvers
{
    p
    {
        solver          GAMG;
        tolerance       0;
        relTol          0.05;
        smoother        GaussSeidel;
        cacheAgglomeration true;
        nCellsInCoarsestLevel 100;
        agglomerator    faceAreaPair;
        mergeLevels     1;
    }

    pFinal
    {
        $p;
        smoother        DICGaussSeidel;
        tolerance       1e-06;
        relTol          0.01;
    }

    "(U|k|nuTilda)"
    {
        //solver          PBiCGStab;
        //preconditioner  DILU;
        solver          smoothSolver;
        smoother        symGaussSeidel;
        tolerance       1e-08;
        relTol          0.1;
        maxIter         5;
    }

    "(U|k|nuTilda)Final"
    {
        $U;
        tolerance       1e-08;
        relTol          0.05;
    }
}

PIMPLE
{
    nOuterCorrectors 1;
    nCorrectors     3;
    nNonOrthogonalCorrectors 0;
    //pRefCell        1001;
    //pRefValue       0;
}

PISO
{
    nOuterCorrectors 1;
    nCorrectors     3;
    nNonOrthogonalCorrectors 0;
    //pRefCell        1001;
    //pRefValue       0;
}

// ************************************************************************* //


""")

# ────────────────────────── Generate profile and plane ──────────────────────


def generate_line_points(start, end, n_points):
    """
    Generate n_points between start and end, inclusive.
    start, end are 3-tuples (x,y,z).
    """
    x0, y0, z0 = start
    x1, y1, z1 = end
    points = []
    for i in range(n_points):
        t = i / (n_points - 1) if n_points > 1 else 0
        x = x0 + t * (x1 - x0)
        y = y0 + t * (y1 - y0)
        z = z0 + t * (z1 - z0)
        points.append(f"({x:.6f} {y:.6f} {z:.6f})")
    return "\n            ".join(points)

def generate_include_func(n_profile, n_plane) -> str:
    includes = []
    for i in range(1, n_profile+1):
        ProfileName=f"Profile{i}"
        includes.append(f'#include "{ProfileName}"')

    for i in range(1, n_plane+1):
        PlaneName=f"Plane{i}"
        includes.append(f'#include "{PlaneName}"')

    functions = "\n    ".join(includes)
    return functions

def create_profile_txt(n_profile: int, profile_out: Path) -> None:

    for i in range(1, n_profile+1):
        print(f"\n--- Configuring Profile{i} ---")

        start = tuple(map(float, get_input("  Start point (x y z)", "0 0 0").split()))
        end   = tuple(map(float, get_input("  End point (x y z)", "0 0 0").split()))
        n_points = int(get_input("  Number of points along the probe line", "5"))
        fields = get_input("Field write {U or p or U p}", "U")
        while fields not in FIELD_MAP:
            fields = get_input("  -> Please choose U or p or U p", "U")
        N_profile = get_input("Field write interval",    "10")
        t_start = get_input("  Time start", "0")

        # generate probe locations
        points = generate_line_points(start, end, n_points)

        # fill template
        profile_text = TP_PROFILE.format(
            ProfileName=f"Profile{i}",
            N_profile=N_profile,
            t_start=t_start,
            fields=fields,
            points=points
        )

        out_path = profile_out.parent / f"Profile{i}"
        out_path.write_text(profile_text)
        print(f"✓ Profile written to {out_path.resolve()}")

def create_plane_txt(N_plane: int, plane_out: Path) -> None:
    for i in range(1, N_plane+1):
        print(f"\n--- Configuring Plane{i} ---")
        
        N_plane = get_input("Plane write interval",    "10")
        t_start = get_input("  Time start", "0.000000")
        t_end = get_input("  Time end", "10.000000")
        fields = get_input("Field write {U or p or U p}", "U")
        while fields not in FIELD_MAP:
            fields = get_input("  -> Please choose U or p or U p", "U")        
        plane_point = "(" + " ".join(get_input("  Point in plane (x y z)", "0 0 0").split()) + ")"
        plane_normal = "(" + " ".join(get_input("  Normal vector to plane (x y z)", "0 0 0").split()) + ")"

        # fill template
        plane_text = TP_PLANE.format(
            PlaneName=f"Plane{i}",
            N_plane=N_plane,
            t_start=t_start,
            t_end=t_end,
            fields=fields,
            plane_point=plane_point,
            plane_normal=plane_normal
        )

        out_path = plane_out.parent  / f"Plane{i}"
        out_path.write_text(plane_text)
        print(f"✓ Plane written to {out_path.resolve()}")

# ───────────────────────── Check if the path existss ─────────────────────────

def existing_path(msg: str) -> Path:
    while True:
        p = Path(input(f"{msg}: ").strip())
        if p.exists():
            return p
        print("  ↳ file not found, try again.")


# load user inputs from a JSON file
def load_input(json_path=None):
    
    inputs = {}

    if json_path is None:
        return inputs

    path = Path(json_path)
    print(f'path {path}')
    if not path.exists():
        raise FileNotFoundError(f"Input file not found: {path}")

    try:
        with path.open("r", encoding="utf-8") as f:
            inputs = json.load(f)
            
    except json.JSONDecodeError as e:
        print(f"❌ JSON syntax error in {path}: {e} .. use a JSON validator")
        sys.exit("Invalid JSON — stopping.")

    if not isinstance(inputs, dict):
        raise ValueError("Input JSON file must contain a JSON object!")

    return inputs
        
# ────────────────────────── main routine ──────────────────────────
def main(config: Dict[str, Any]) -> None:
    # ───────────────────────── Input Region ─────────────────────────

    if 'geographic_extent' in config:
        geographic_extent = config['geographic_extent']

        #
        # geographic_extent supplied in input file
        #
        
        try:
            larger = geographic_extent["larger_region"]
            lat = float(larger["lat"])
            lon = float(larger["lon"])
            r = float(larger["radius"])
            region_of_interest = geographic_extent["region_of_interest"]
            lat_min = float(region_of_interest["min_lat"])
            lon_min = float(region_of_interest["min_lon"])
            lat_max = float(region_of_interest["max_lat"])
            lon_max = float(region_of_interest["max_lon"])                        
            
        except KeyError as e:
            raise ValueError(f"geographic_extent missing key: {e}") from None
        except (TypeError, ValueError):
            raise ValueError("lat, long, radius, min_lat, min_lon, max_lat, max_lon must be numeric")

    else:

        #
        # user manual entry of geometry
        #
        
        print("Enter centre coordinates and radius of the larger circular region")
        lon = ask_float("  longitude (deg)")
        lat = ask_float("  latitude  (deg)")
        r   = ask_float("Radius of circle (metres)", "default = 500")

        print("Enter bounding-box coordinates for Region of Interest (ROI):")
        lon_min = ask_float("  lon min: ")
        lat_min = ask_float("  lat min: ")
        lon_max = ask_float("  lon max: ")
        lat_max = ask_float("  lat max: ")

    # otput files containing building footprints for larger and roi
    
    out_path = Path(r"inventoryTotal.geojson")
    out_file = Path(r"inventoryROI.geojson")

    if 'brails_options' in config:
        brails_options = config['brails_options']

        #
        # brails_options supplied in input file
        #
        
        try:
            scraper_choice = brails_options["scraper"]
            dflt = float(brails_options["default_building_height"])
            
        except KeyError as e:
            raise ValueError(f"brails_options missing key: {e}") from None
        except (TypeError, ValueError):
            raise ValueError("default_building_height must be a number")        

    else:

        # brails options from user
        scraper_choice = ask_choice("Scraper source {USA | OSM | MS}", ["USA", "OSM", "MS"], "USA")
        dflt = ask_float("Default height (m) in case the inventory has no heights")        

    
    # number of perimeter points; 36 ≈ every 10°
    n_pts = 36
    geod = Geod(ellps="WGS84")
    bearings = [i * 360 / n_pts for i in range(n_pts)]
    verts = [geod.fwd(lon, lat, brg, r)[:2] for brg in bearings]
    verts.append(verts[0])  # close polygon

    flat_coords = tuple(coord for ll in verts for coord in ll)

    # BRAILS workflow
    importer = Importer()
    RegionBoundary = importer.get_class("RegionBoundary")
    regionT = RegionBoundary({"type": "locationPolygon", "data": flat_coords})
    scraper_map = {
        "USA": "USA_FootprintScraper",
        "OSM": "OSM_FootprintScraper",
        "MS":  "MS_FootprintScraper",
    }
    #Scraper = importer.get_class("USA_FootprintScraper")
    Scraper = importer.get_class(scraper_map[scraper_choice])
    Tfootprints = Scraper({"length": "m"}).get_footprints(regionT)

    Tfootprints.write_to_geojson(out_path)
    print(f"✓  GeoJSON written to {out_path.resolve()}")
    


    # RegionBoundary requires a flat tuple with an even number of values
    bbox_flat = (lon_min, lat_min, lon_max, lat_max)
    regionR = RegionBoundary({"type": "locationPolygon", "data": bbox_flat})

    
    
    scraper = Scraper({"length": "m"})  

    # ---------------------------------------------------------------- DOWNLOAD & SAVE
    inventory = scraper.get_footprints(regionR)
    inventory.write_to_geojson(out_file)

    print(f"✓  Footprints saved to {out_file.resolve()}")

    full = out_path
    roi  = out_file
    
    # ───────────────────────── Wind Direction ─────────────────────────

    if 'wind_direction' in config:
        try:
            angle_deg = config['wind_direction']
        except (TypeError, ValueError):
            raise ValueError("wind_direction must be a float")                    
    else:
        angle_deg = ask_angle()        # θ anti-clockwise from +x

    # ───────────────────────── Write blockMesh and STL files ─────────────────────────
    if 'case_folder' in config:
        case = Path(config['case_folder'])
    else:
        case = ask_dir("Case folder")

    tri = case/"constant"/"triSurface"
    tri.mkdir(parents=True, exist_ok=True)
    roi_stl, rest_stl = tri/"ROI.stl", tri/"Surrounding.stl"
    # dflt = ask_float("Default height (m) in case the inventory has no heights")
    fld="buildingheight";
    
    # --- load, CRS -------------------------------------------------------
    gdf_roi  = gpd.read_file(roi)
    gdf_full = gpd.read_file(full)
    
    if gdf_roi.empty: sys.exit("ROI empty.")
    lon0,lat0 = (gdf_roi.geometry.iloc[0].exterior.coords[0]
                 if isinstance(gdf_roi.geometry.iloc[0], geom.Polygon)
                 else gdf_roi.geometry.iloc[0].geoms[0].exterior.coords[0])
    epsg = utm_epsg(lon0,lat0)
    gdf_roi  = to_metric(gdf_roi ,epsg)
    gdf_full = to_metric(gdf_full,epsg)

    # --- rotate by −θ to align wind -> +x --------------------------------
    gdf_roi .geometry = gdf_roi .geometry.rotate(-angle_deg, origin=(0,0), use_radians=False)
    gdf_full.geometry = gdf_full.geometry.rotate(-angle_deg, origin=(0,0), use_radians=False)

    # --- split remaining --------------------------------------------------
    rest = gdf_full.loc[~gdf_full.geometry.apply(lambda g: g.wkb in {w.wkb for w in gdf_roi.geometry})].copy()

    # --- heights ----------------------------------------------------------
    for gdf in (gdf_roi, rest):
        if fld not in gdf.columns: gdf[fld]=np.nan
        gdf[fld]=pd.to_numeric(gdf[fld],errors="coerce")
    gdf_roi = nearest_fill(gdf_roi,fld,dflt)
    rest    = nearest_fill(rest   ,fld,dflt)
    hmax = max(gdf_roi[fld].max(), rest[fld].max())
    print(f"Hmax = {hmax}")


    if 'computational_domain' in config:
        computational_domain = config['computational_domain']

        try:
            domain_extents = computational_domain["domain_extents"]
            inmult   = float(domain_extents["inlet_multiplier"])
            outmult  = float(domain_extents["outlet_multiplier"])
            sidmult  = float(domain_extents["side_multiplier"])
            topmult  = float(domain_extents["top_multiplier"])
            
        except KeyError as e:
            raise ValueError(f"computational_domain missing key: {e}") from None
        except (TypeError, ValueError):
            raise ValueError("multipliers in domain_extents must be floats")

    else:    
    
        print(f"Specify the domain extents (i.e. the distance to the boundaries from each end of the region)")
        inmult   = ask_mult("Inlet  distance multiplier (i.e. xHmax)", 5.0)
        outmult  = ask_mult("Outlet distance multiplier (i.e. xHmax)", 15.0)
        sidmult    = ask_mult("Side   distance multiplier (i.e. xHmax)", 5.0)
        topmult     = ask_mult("Top    distance multiplier (i.e. xHmax)", 15.0)
        # --- origin shift -----------------------------------------------------

    all_geom = pd.concat([gdf_roi,rest]).geometry
    xs = np.concatenate([np.asarray(p.exterior.xy[0] if isinstance(p,geom.Polygon)
                                    else p.convex_hull.exterior.xy[0]) for p in all_geom])
    ys = np.concatenate([np.asarray(p.exterior.xy[1] if isinstance(p,geom.Polygon)
                                    else p.convex_hull.exterior.xy[1]) for p in all_geom])
    xshift = xs.min()-inmult*hmax
    yshift = ys.min()-sidmult*hmax
    for gdf in (gdf_roi, rest):
        gdf.set_geometry(gdf.geometry.translate(xoff=-xshift,yoff=-yshift), inplace=True)

    # --- STL write --------------------------------------------------------
    def mesh(gdf):
        parts=[]
        for pg,h in zip(gdf.geometry,gdf[fld]):
            if isinstance(pg,geom.MultiPolygon): parts.extend(extrude(p,h) for p in pg.geoms)
            else: parts.append(extrude(pg,h))

            m=trimesh.util.concatenate(parts)
        # m.remove_duplicate_faces(); m.remove_degenerate_faces(); m.fill_holes()
        return m
    write_stl_ascii(mesh(gdf_roi), roi_stl,  "ROI")
    write_stl_ascii(mesh(rest),    rest_stl, "Surrounding")
    print("✓ STL files created.")
    
    # --- compute bounds ---------------------------------------------------
    verts=np.vstack([trimesh.load_mesh(str(roi_stl)).vertices,
                     trimesh.load_mesh(str(rest_stl)).vertices])
    xmin,ymin,_ = verts.min(axis=0)
    xmax,ymax,_ = verts.max(axis=0)
    x0,x1,y0,y1 = make_bounds(xmin,xmax,ymin,ymax,hmax,inmult,outmult,sidmult,"W")   # inlet at west after rotation
    z0,z1 = 0.0, topmult*hmax

    if 'computational_domain' in config and 'boundary_mesh_cell_size' in config['computational_domain']:
        try:
            cell = config['computational_domain']['boundary_mesh_cell_size']
        except (TypeError, ValueError):
            raise ValueError("boundary_mesh_cell_size must be a float")
    else:
        cell=ask_mult("Boundary Mesh Cell size",4.0)
        
    nx,ny,nz = (math.ceil((x1-x0)/cell),
                math.ceil((y1-y0)/cell),
                math.ceil((z1-z0)/cell))

    # moved side_choice up here
    if ('computational_domain' in config
        and 'boundary_conditions' in config['computational_domain']
        and 'side' in config['computational_domain']['boundary_conditions']
        ):
        side_choice = config['computational_domain']['boundary_conditions']['side']
    else:
        side_choice = ask_side()        
    
    write_bmd(x0,x1,y0,y1,z0,z1,nx,ny,nz,side_choice,
              case/"system"/"blockMeshDict")
    
    print("✓ blockMeshDict written.")
    print(f"Applied rotation −{angle_deg}° so wind aligns with +x.")
    print(f"Extents: ({x0},{y0},{z0}) to ({x1},{y1},{z1})")
    # ───────────────────────── Write snappyHexMesh ─────────────────────────

    # args = cli() NOT USED!
    
    print("Add refinement regions")

    boxes: List[Tuple[str, Tuple[float, float, float, float, float, float], str]] = []

    if 'computational_domain' in config and 'refinement_regions' in config['computational_domain']:

        regions = config['computational_domain']['refinement_regions']
        
        for region in regions:
            try:
                name = region["name"]
                level = region["level"]
                coords = (
                    float(region["x_min"]),
                    float(region["y_min"]),
                    float(region["z_min"]),
                    float(region["x_max"]),
                    float(region["y_max"]),
                    float(region["z_max"]),
                )

                boxes.append((name, coords, str(level)))
        
            except KeyError as e:
                raise ValueError(f"Missing key {e} in refinement region: {region}") from e
            except (TypeError, ValueError):
                raise ValueError("coords x_min, ... z_max must be floats")
        
    else:

        # 1. interactive loop for boxes
        idx = 1
        while True:
            res = ask_box(idx)
            if res is None:
                break
            boxes.append(res)
            idx += 1

        if not boxes:
            print("No boxes entered")

    # 2. ask ROI / Surrounding levels
    print("\n--- Surface refinement levels ---")

    if 'computational_domain' in config and 'roi_surface_level' in config['computational_domain']:
        roi_level_input = str(config['computational_domain']['roi_surface_level'])
    else:
        roi_level_input = input("  ROI surface level [(auto)]: ").strip()

    if 'computational_domain' in config and 'surrounding_surface_level' in config['computational_domain']:
        sur_level_input = str(config['computational_domain']['surrounding_surface_level'])
    else:
        sur_level_input = input("  Surrounding surface level [(auto)]: ").strip()

    if 'computational_domain' in config and 'cells_between_levels' in config['computational_domain']:
        try: 
            ncellblvl = int(config['computational_domain']['cells_between_levels'])
        except (TypeError, ValueError):
            raise ValueError("cells_between_levels must be an int")
    else:        
        ncellblvl = int(ask_mult("No. of cells between each level",5))

    
    # 3. determine defaults if needed
    max_box_lvl = max_numeric_level([b[2] for b in boxes])   # b[2] now works
    sur_level = sur_level_input or f"{max_box_lvl + 1}"
    roi_level = roi_level_input or f"{max_box_lvl + 2}"

    # feature levels (single int) → take highest numeral in each level string
    roi_feat_level = max(map(int, re.findall(r"\d+", roi_level)))
    sur_feat_level = max(map(int, re.findall(r"\d+", sur_level)))

    # 4. create geometry / refinement strings
    geom_entries = []
    ref_entries  = []
    for name, (xmin, ymin, zmin, xmax, ymax, zmax), lev in boxes:
        geom_entries.append(GEOM_BOX.format(name=name,
                                            xmin=xmin, ymin=ymin, zmin=zmin,
                                            xmax=xmax, ymax=ymax, zmax=zmax))
        ref_entries.append(REFINE_BOX.format(name=name, level=lev))

    dict_text = TEMPLATE.format(
        roi_stl="ROI",
        sur_stl="Surrounding",
        geometry_entries="\n".join(geom_entries),
        refinement_entries="\n".join(ref_entries),
        roi_level=roi_level,
        sur_level=sur_level,
        ncellblvl=ncellblvl,
        roi_feat_level=roi_feat_level,
        sur_feat_level=sur_feat_level,
    )

    Path(case/"system"/"snappyHexMeshDict").write_text(dict_text)
    Path(case/"system"/"surfaceFeaturesDict").write_text(TP_surfFeat)
    print(f"✓  snappyHexMeshDict written to {Path(case/"system"/"snappyHexMeshDict").resolve()}")
    print(f"   ROI surface level        : {roi_level}")
    print(f"   Surrounding surface level: {sur_level}")

    # ───────────────────────── Transport Properties and Framework ─────────────────────────


    if 'computational_domain' in config and 'cells_between_levels' in config['computational_domain']:
        try: 
            nu = float(config['computational_domain']['kinematic_viscosity'])
        except (TypeError, ValueError):
            raise ValueError("kinematic_viscosity must be a float")
    else:            
        nu = get_input("Kinematic viscosity", "1.5e-05")

    transport_properties_write = TP_transportProperty.format(
        nu=nu
    )
    ftp = Path(case/"constant"/"transportProperties")
    ftp.parent.mkdir(parents=True, exist_ok=True)
    ftp.write_text(transport_properties_write)


    if 'computational_domain' in config and 'cells_between_levels' in config['computational_domain']:
        try: 
            nu = float(config['computational_domain']['kinematic_viscosity'])
        except (TypeError, ValueError):
            raise ValueError("kinematic_viscosity must be a float")
    else:            
        nu = get_input("Kinematic viscosity", "1.5e-05")    

    side = side_choice

    fld2 = Path(case/"constant"/"turbulenceProperties")
        
    if 'computational_domain' in config and 'boundary_conditions' in config['computational_domain']:
        boundary_conditions = config['computational_domain']['boundary_conditions']
        try:
            ground_style=boundary_conditions['ground_style']
            roi_style=boundary_conditions['roi_style']
            surround_style=boundary_conditions['surround_style']
            
            inlet=boundary_conditions['inlet']
            fw=inlet['framework']
            Uref=float(inlet['Uref'])
            Href=float(inlet['Href'])
            z0=float(inlet['z0'])
            if fw == "les" or fw == "LES":
                fw = "les"
                inflow=inlet['inflow']
                
        except KeyError as e:
            raise ValueError(f"Missing key {e} in boundary_conditions: {region}") from e
        except (TypeError, ValueError):
            raise ValueError("Uref, Href, z0 must be floats")            
                
    else:
        print("✓ constant/transportProperties written.")
        while True:
            fw = input("Framework? [RANS / LES]: ").strip().lower()
            if fw in ("rans", "les"):
                break

        if fw == "les":
            # meanABL or turbulent inflow?
            inflow = ask("LES inlet type {meanABL|turbulent}", "meanABL").lower()
            while inflow not in ("meanabl", "turbulent"):
                inflow = ask(" choose meanABL or turbulent", "meanABL").lower()
        
            ground_style = ask("Ground wall function {rough|smooth}", "smooth").lower()
            while ground_style not in ("rough", "smooth"):
                ground_style = ask(" choose rough/smooth", "smooth").lower()

            roi_style = ask("ROI wall function {rough|smooth}", "smooth").lower()
            while roi_style not in ("rough", "smooth"):
                roi_style = ask(" choose rough/smooth", "smooth").lower()

            surround_style = ask("Surrounding wall function {rough|smooth}", "smooth").lower()
            while surround_style not in ("rough", "smooth"):
                surround_style = ask(" choose rough/smooth", "smooth").lower()

            Uref = ask("Reference wind speed Uref (m/s)", "10")
            Href = ask("Reference height Zref (m)", "10")
            z0   = ask("Roughness length z0 (m)", "0.1")

        else:
            
            Uref = ask("Reference wind speed Uref (m/s)", "10")
            Href = ask("Reference height Zref (m)", "10")
            z0   = ask("Roughness length z0 (m)", "0.1")

            side = side_choice

            ground_style = ask("Ground wall function {rough|smooth}", "smooth").lower()
            while ground_style not in ("rough", "smooth"):
                ground_style = ask(" choose rough/smooth", "smooth").lower()

            roi_style = ask("ROI wall function {rough|smooth}", "smooth").lower()
            while roi_style not in ("rough", "smooth"):
                roi_style = ask(" choose rough/smooth", "smooth").lower()

            surround_style = ask("Surrounding wall function {rough|smooth}", "smooth").lower()
            while surround_style not in ("rough", "smooth"):
                surround_style = ask(" choose rough/smooth", "smooth").lower()

    if fw == "les":
        
        # write turbulenceProperties
        fld2.parent.mkdir(parents=True, exist_ok=True)
        fld2.write_text(TP_LES)
        print("✓ turbulenceProperties (LES) written.")

        fld1 = Path(case/"0")
        fld1.mkdir(parents=True, exist_ok=True)

        # choose U body
        if inflow == "meanabl":
            Ubody = body_U_meanABL(SIDE_MAP[side], Uref, Href, z0)
        else:
            Ubody = body_U_DFM(SIDE_MAP[side], Uref)
            
        (fld1 / "U").write_text(Ubody)
        fld1.joinpath("nut").write_text(
            body_nut(SIDE_MAP[side], ground_style, z0,Uref,Href, roi_style, surround_style))
        
        (fld1 / "p").write_text(
            P_BODY % (SIDE_MAP[side], SIDE_MAP[side])
        )

        print(f"✓ fields written to {fld1.resolve()}")
        #return  # LES path ends here

    else:

        # ───────────── RANS branch  ─────────────
        fld2.parent.mkdir(parents=True, exist_ok=True)
        fld2.write_text(TP_RANS)
        print("✓ turbulenceProperties (RANS) written.")
        
        fld1 = Path(case/"0")
        fld1.mkdir(parents=True, exist_ok=True)
        
        fld1.joinpath("U").write_text(
            body_U_meanABL(SIDE_MAP[side], Uref, Href, z0))
        
        fld1.joinpath("k").write_text(
            body_k(SIDE_MAP[side], Uref, Href, z0))
        
        fld1.joinpath("epsilon").write_text(
            body_eps(SIDE_MAP[side], Uref, Href, z0))
        
        fld1.joinpath("nut").write_text(
            body_nut(SIDE_MAP[side], ground_style, z0,Uref,Href, roi_style, surround_style))
        
        fld1.joinpath("p").write_text(
            P_BODY % (SIDE_MAP[side], SIDE_MAP[side])
        )
        
        print(f"✓ fields written to {fld1.resolve()}")
        
        
    # ───────────────────────── Inputs for ControlDict ─────────────────────────
    
    lib_TINF = ""
    if fw == "les":
        
        if inflow == "turbulent":
            lib_TINF = '"libturbulentInflow.so"'

        if 'computational_domain' in config and 'control_dict' in config['computational_domain']:
            control_dict = config['computational_domain']['control_dict']
            try: 
                end_T=float(control_dict['end_time'])
                deltaT_sim=float(control_dict['initial_deltaT_sim'])
                maxDeltaT = float(control_dict['max_deltaT_sim']) 
                adjust_time=control_dict['adjust_time']
                deltaT_write=float(control_dict['deltaT_write'])
                les_algorithm=control_dict['solver']
                maxCo = float(control_dict['max_courant'])
                n_profile = int(control_dict['num_wind_profiles'])
                n_plane = int(control_dict['num_section_planes'])              

            except KeyError as e:
                raise ValueError(f"Missing key {e} in control_dict") from e
            except (TypeError, ValueError):
                raise ValueError("float (time quantities and courant number) or int expected")
                              
        else:
            
            les_algorithm = get_input("LES algorithm {pisoFoam | pimpleFoam}", "pisoFoam")
            while les_algorithm not in LES_ALGORITHM_MAP:
                les_algorithm = get_input("  -> Please choose pisoFoam or pimpleFoam", "pisoFoam")
            end_T = get_input("End time for the simulation",    "100.000000")
            deltaT_sim = get_input("Initial time step of the simulation",    "0.0050")
            adjust_time = input("Adjust time step according to maximum Courant number? [yes / no]: ").strip()
            deltaT_write = get_input("Write interval (Run Time)",    "1") 
            maxCo = get_input("Max Courant number allowed",    "1.00")
            maxDeltaT = get_input("Maximum time step allowed in transient simulation",    "0.01000") 
            n_profile = int(get_input("Number of wind profiles for recording", "0"))
            n_plane = int(get_input("Number of section planes for recording", "0"))
        
        functions = generate_include_func(n_profile, n_plane)

        # write controlDict 
        les_controldict = TP_LESC.format(
            les_algorithm = les_algorithm, 
            end_T = end_T,
            deltaT_sim = deltaT_sim,
            adjust_time = adjust_time,   
            deltaT_write = deltaT_write,
            maxCo = maxCo,
            maxDeltaT = maxDeltaT,
            lib_TINF = lib_TINF,
            functions = functions
        )
        Path(case/"system"/"controlDict").parent.mkdir(parents=True, exist_ok=True)
        Path(case/"system"/"controlDict").write_text(les_controldict)
        Path(case/"system"/"fvSolution").parent.mkdir(parents=True, exist_ok=True)
        Path(case/"system"/"fvSolution").write_text(TP_fvSol_LES)
        Path(case/"system"/"fvSchemes").parent.mkdir(parents=True, exist_ok=True)
        Path(case/"system"/"fvSchemes").write_text(TP_fvSch_LES)
        print(f"✓ system/controlDict written to {Path(case/"system"/"controlDict").resolve()}")
        # write profile and plane recorder
        create_profile_txt(n_profile, Path(case/"system"/"Profile"))
        create_plane_txt(n_plane, Path(case/"system"/"Plane"))

    else:

        # Control dict if RANS
        
        if 'computational_domain' in config and 'control_dict' in config['computational_domain']:
            control_dict = config['computational_domain']['control_dict']
            try: 
                end_T=float(control_dict['end_time'])
                deltaT_sim=float(control_dict['deltaT_sim'])
                deltaT_write=float(control_dict['deltaT_write'])                
                
            except KeyError as e:
                raise ValueError(f"Missing key {e} in control_dict") from e
            except (TypeError, ValueError):
                raise ValueError("Uref, Href, z0 must be floats")                              
        else:
                              
            end_T = get_input("End time for the simulation",    "1000")
            deltaT_sim = get_input("Time step of the simulation",    "1") 
            deltaT_write = get_input("Write interval (Time step)",    "100")
            
        # write controlDict 
        rans_text = TP_RANSC.format(
            end_T = end_T,
            deltaT_sim =deltaT_sim,
            deltaT_write = deltaT_write
        )
        Path(case/"system"/"controlDict").parent.mkdir(parents=True, exist_ok=True)
        Path(case/"system"/"controlDict").write_text(rans_text)
        Path(case/"system"/"fvSolution").parent.mkdir(parents=True, exist_ok=True)
        Path(case/"system"/"fvSolution").write_text(TP_fvSol_RANS)
        Path(case/"system"/"fvSchemes").parent.mkdir(parents=True, exist_ok=True)
        Path(case/"system"/"fvSchemes").write_text(TP_fvSch_RANS)
        print(f"✓ system/controlDict written to {Path(case/"system"/"controlDict").resolve()}")

        
    if 'number_of_processors' in config:
        nproc = int(config['number_of_processors'])
    else:
        while True:
            txt = input("Number of processors (positive int) : ").strip()
            if txt.isdigit() and int(txt) > 0:
                nproc = int(txt)
                break
            print("  ↳ please enter a positive integer")

# ------------------------------------------------------------------ template for decomposePar
    DECOMP = f"""/*--------------------------------*- C++ -*----------------------------------*\\
    | =========                 |                                                 |
    | \\      /  Field           | OpenFOAM: The Open Source CFD Toolbox          |
    |  \\    /   Operation       | Version:  10                                   |
    |   \\  /    And             | Web:      www.openfoam.org                     |
    |    \\/     Manipulation    |                                                |
    \\*---------------------------------------------------------------------------*/

    FoamFile
    {{
        version     2.0;
        format      ascii;
        class       dictionary;
        object      decomposeParDict;
    }}

    numberOfSubdomains  {nproc};

    method              scotch;   // automatic graph partitioning

    simpleCoeffs        {{ n (1 1 1); delta 0.001; }}
    hierarchicalCoeffs  {{ n (1 1 1); delta 0.001; order xyz; }}
    manualCoeffs        {{ dataFile ""; }}

    distributed         no;
    roots               ();
    """


    outd = Path(case/"system"/"decomposeParDict") 
    outd.parent.mkdir(parents=True, exist_ok=True)
    outd.write_text(DECOMP)
    print(f"✓ decomposeParDict written to {outd.resolve()}")
    print(f"  ( {nproc} subdomains, method = scotch )")

    # ───────────────────────── Get TINF file ─────────────────────────

    if fw == "les":
        if inflow == "turbulent":

            if 'computational_domain' in config and 'boundary_conditions' in config['computational_domain']:
                #inlet = config['computational_domain']['inlet']
                src = inlet['path_TINF_file']
                    
            else:
                src = existing_path("Path to CSV file for TINF")

            
            Path(case/"constant"/"boundaryData"/"inlet"/"points").parent.mkdir(parents=True, exist_ok=True)
            Path(case/"constant"/"boundaryData"/"inlet"/"U").parent.mkdir(parents=True, exist_ok=True)
            Path(case/"constant"/"boundaryData"/"inlet"/"R").parent.mkdir(parents=True, exist_ok=True)
            Path(case/"constant"/"boundaryData"/"inlet"/"L").parent.mkdir(parents=True, exist_ok=True)
            pts_out = Path(case/"constant"/"boundaryData"/"inlet"/"points")
            u_out   = Path(case/"constant"/"boundaryData"/"inlet"/"U")
            r_out   = Path(case/"constant"/"boundaryData"/"inlet"/"R")
            l_out   = Path(case/"constant"/"boundaryData"/"inlet"/"L")

    # ---- read first 17 columns -----------------------------------------
            try:
                #if src.suffix.lower() == ".csv":
                df = pd.read_csv(src, header=None, usecols=range(17))
                #else:
                 #   sys.exit("Input must be .csv")
            except Exception as exc:
                sys.exit(f"Could not read {src}: {exc}")

            df = df.apply(pd.to_numeric, errors="coerce").dropna()
            if df.shape[1] < 17 or df.empty:
                sys.exit("Need at least 17 numeric columns with no blanks.")

            n = len(df)
            z_vals = df.iloc[:, 0]
            u_vals = df.iloc[:, 1]
            r_vals = df.iloc[:, 2:8]   # six
            l_vals = df.iloc[:, 8:17]  # nine

    # ---- Write list files ------------------------------------
            def write_list(path: Path, iterable, fmt):
                with path.open("w") as f:
                    f.write(f"{n}\n(\n")
                    for itm in iterable:
                        f.write(fmt(itm) + "\n")
                    f.write(")\n;\n")

    # points – last point has y = 5
            with pts_out.open("w") as f:
                f.write(f"{n}\n(\n")
                for i, z in enumerate(z_vals):
                    if i == n - 1:
                        f.write(f"(0 5 {z:g})\n")
                    else:
                        f.write(f"(0 0 {z:g})\n")
                f.write(")\n;\n")
            print(f"✓ points → {pts_out.resolve()}")

    # U
            write_list(u_out, u_vals, lambda u: f"{u:g}")
            print(f"✓ U      → {u_out.resolve()}")

    # R
            write_list(r_out, r_vals.itertuples(index=False),
                    lambda row: "(" + " ".join(f"{v:g}" for v in row) + ")")
            print(f"✓ R      → {r_out.resolve()}")

    # L
            write_list(l_out, l_vals.itertuples(index=False),
                    lambda row: "(" + " ".join(f"{v:g}" for v in row) + ")")
            print(f"✓ L      → {l_out.resolve()}")

    Path(case/"Community.foam").touch()

if __name__ == "__main__":

    json_inputfile_path = sys.argv[1] if len(sys.argv) > 1 else None
    input_config = load_input(json_inputfile_path)
    main(input_config)
    

    
