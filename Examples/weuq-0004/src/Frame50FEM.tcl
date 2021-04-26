#units kN, m

pset k 2.6e6
pset rho 250.;  #kg/m^3

set g 9.81;  # m/s
set b 40.
set d 40.
set h 200.
set numStory 50
set bW [expr $b*$d*$h*$rho*$g/1000.]; # building Weight
set fW [expr $bW/(1.*$numStory)]; # floor Weight
set fM [expr $fW/$g]
set sH [expr $h/(1.*$numStory)];  # story Height
set sM 1.0e-3

puts "floorWeight: $fW"

model Basic -ndm 3 -ndf 6
node 1 0. 0. 0.
fix 1 1 1 1 1 1 1
set E 29000.
set A 1e4
set I [expr $k*$sH*$sH*$sH/(12.*$E)]
geomTransf Linear 1 0 1 0
set G 1e4
set J 1e8

for {set i 1; set j 2} {$i <= $numStory} {incr i 1; incr j 1} {
    node $j 0. 0. [expr $i*$sH] -mass $fM $fM $fM $sM $sM $sM
    fix $j 0 0 1 1 1 0
    element elasticBeamColumn $i $i $j $A $E $G $J $I $I 1
}

set numEigen 3
set lambda [eigen $numEigen]
for {set i 0} {$i < $numEigen} {incr i 1} {
    set omega [expr sqrt([lindex $lambda $i])]
    set T [expr 2*3.14159/$omega]
    puts "$i $T [expr 1/$T]"
}

recorder EnvelopeNode -file disp.out -nodeRange 1 [expr $numStory+1]  -dof 1 2 disp
recorder NodeRMS -file accel.out -nodeRange 1 [expr $numStory+1] -dof 1 2 accel
recorder EnvelopeElement -file forces.out -eleRange 1 $numStory forces
    
puts "$fW $k"