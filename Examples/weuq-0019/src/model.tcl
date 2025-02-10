model BasicBuilder -ndm 2 -ndf 2
node 2 0 144 -mass 0.372971 0.372971  0.0  0.0  0.0 3.72971e-11 
node 3 0 288 -mass 0.372971 0.372971  0.0  0.0  0.0 3.72971e-11 
node 1 0 0
fix  1 1 1
uniaxialMaterial Steel01 1 1e+06 100 0.1
uniaxialMaterial Steel01 2 1e+06 100 0.1
uniaxialMaterial Steel01 4 1e+06 100 0.1
uniaxialMaterial Steel01 5 1e+06 100 0.1
element zeroLength 1 1 2 -mat 1 2  -dir 1 2  -doRayleigh
element zeroLength 2 2 3 -mat 4 5  -dir 1 2  -doRayleigh
loadConst -time 0.0

