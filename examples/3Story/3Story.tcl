#units kip, in

# parameters
pset storyK  400.0
pset bldgW  6485.0

# constants
set g 386.1;  
set b 1440.
set d 2160.
set h 468.
set numStory 3

# derived parameters
set fW [expr $bldgW/(1.*$numStory)]; # floor Weight
set fM [expr $fW/$g]
set sH [expr $h/(1.*$numStory)];  # story Height
set sM 1.0e-3

# build fe model
model Basic -ndm 3 -ndf 6
node 1 0. 0. 0.
fix 1 1 1 1 1 1 1
set E 29000.
set A 1e4
set I [expr $storyK * $sH * $sH * $sH / (12.*$E)]
geomTransf Linear 1 0 1 0
set G 1e4
set J 1e8

for {set i 1; set j 2} {$i <= $numStory} {incr i 1; incr j 1} {
    node $j 0. 0. [expr $i*$sH] -mass $fM $fM $fM $sM $sM $sM
    fix $j 0 0 1 1 1 0
    element elasticBeamColumn $i $i $j $A $E $G $J $I $I 1
}

#set numEigen 3
#set lambda [eigen $numEigen]
#puts $fW
#for {set i 0} {$i < $numEigen} {incr i 1} {
#    set omega [expr sqrt([lindex $lambda $i])]
#    set T [expr 2*3.14159/$omega]
#    puts "$i $T [expr 1/$T]"
#}
#wipeAnalysis

