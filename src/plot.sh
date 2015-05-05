#!/bin/bash


BIN_DIR="/home/ana/Research/VisionProjectKrang/alvarCode/bin/"

#plot "logging_est0.txt" every ::1::100 using 0:1 with lines, "logging_est1.txt" every ::1::100 using 0:4 with lines

for i in 2 1 0
do
	FILENAME=${BIN_DIR}
        FILENAME+="logging_est"
	FILENAME+="${i}" 
        FILENAME+=".txt"
	gnuplot <<- EOF
        	set title "Object ${i}: X"
        	set terminal pngcairo
        	set output "object${i}_x.png"
        	plot "${FILENAME}" using 0:1 with lines title "Estimated X", "${FILENAME}" using 0:4 with lines title "Raw X"
        	set title "Object ${i}: Y"
        	set output "object${i}_y.png"
        	plot "${FILENAME}" using 0:2 with lines title "Estimated Y", "${FILENAME}" using 0:5 with lines title "Raw Y"
        	set title "Object ${i}: Theta"     
        	set output "object${i}_theta.png"
        	plot "${FILENAME}"  using 0:3 with lines title "Estimated Theta", "${FILENAME}"  using 0:6 with lines title "Raw Theta"
        	set title "Object ${i}: XY position"
        	set output "object${i}_xy.png"
        	plot "${FILENAME}" using 1:2 with lines title "Estimated", "${FILENAME}" using 4:5 with lines title "Raw"

	EOF
done
