set terminal postscript eps color enhanced "Helvetica" 38 size 20,5
set style fill solid 1 border -1 


#Utilization (Box)

#set style boxplot nooutliers
set style boxplot outliers pointtype 6
set style boxplot fraction 1.00
set style data boxplot
set boxwidth 0.2
#set xlabel "Number of Ways for I/O" font "Helvetica,16"

set xlabel "Resources" font "Helvetica,16"
set ylabel "Utilization (%)" font "Helvetica,16"
set key top Left center outside reverse width 2 horizontal font "Helvetica, 12"
set xtics ("All" 1, "CPU" 2, "All Memories" 3, "LM" 4, "LRM" 5, "RRM" 6) font "Helvetica, 12" 
set ytic font "Helvetica, 16" 
#set ytic 0,2
set yrange [-1:105]
set xrange [0:6.5]

set offsets graph 0, 0, 0.05, 0.05 
set grid
#set parametric

N_CP = (system("wc -l filtered_cluster_cp.txt")-1)
N_CP_NONE_ZERO = (system("awk -F, '{if($2 !=0){print $0}}' filtered_cluster_cp.txt | wc -l")-1)
N_FF = (system("wc -l filtered_cluster_cp.txt")-1)
N_FF_NONE_ZERO = (system("awk -F, '{if($2 !=0){print $0}}' filtered_cluster_ff.txt | wc -l")-1)

set terminal pdf
set datafile separator ","
set output 'util.pdf'
plot "filtered_cluster_cp.txt" using (0.8):($2) pointsize 0.5 linecolor rgb '#2ca25f' title sprintf("CP - %i/%i Clusters", N_CP_NONE_ZERO,N_CP) ,\
"filtered_cluster_ff.txt" using (1.2):($2) linecolor rgb '#e5f5f9' pointsize 0.5 title sprintf("FF - %i/%i Clusters", N_FF_NONE_ZERO,N_FF) ,\
"filtered_cluster_cp.txt" using (1.8):($5) linecolor rgb '#2ca25f' pointsize 0.5 notitle,\
"filtered_cluster_ff.txt" using (2.2):($5) linecolor rgb '#e5f5f9' pointsize 0.5 notitle,\
"filtered_cluster_cp.txt" using (2.8):($8) linecolor rgb '#2ca25f' pointsize 0.5 notitle,\
"filtered_cluster_ff.txt" using (3.2):($8) linecolor rgb '#e5f5f9' pointsize 0.5 notitle,\
"filtered_cluster_cp.txt" using (3.8):($10*100/$9) linecolor rgb '#2ca25f' pointsize 0.5 notitle,\
"filtered_cluster_ff.txt" using (4.2):($10*100/$9) linecolor rgb '#e5f5f9' pointsize 0.5 notitle,\
"filtered_cluster_cp.txt" using (4.8):($13*100/$12) linecolor rgb '#2ca25f' pointsize 0.5 notitle,\
"filtered_cluster_ff.txt" using (5.2):($13*100/$12) linecolor rgb '#e5f5f9' pointsize 0.5 notitle,\
"filtered_cluster_cp.txt" using (5.8):($16*100/$15) linecolor rgb '#2ca25f' pointsize 0.5 notitle,\
"filtered_cluster_ff.txt" using (6.2):($16*100/$15) linecolor rgb '#e5f5f9' pointsize 0.5 notitle;


set xrange [0:3.5]

set terminal pdf
set datafile separator ","
set output 'util-high-level.pdf'
plot "filtered_cluster_cp.txt" using (0.8):($2) pointsize 0.5 linecolor rgb '#2ca25f' title sprintf("CP - %i/%i Clusters", N_CP_NONE_ZERO,N_CP) ,\
"filtered_cluster_ff.txt" using (1.2):($2) linecolor rgb '#e5f5f9' pointsize 0.5 title sprintf("FF - %i/%i Clusters", N_FF_NONE_ZERO,N_FF) ,\
"filtered_cluster_cp.txt" using (1.8):($5) linecolor rgb '#2ca25f' pointsize 0.5 notitle,\
"filtered_cluster_ff.txt" using (2.2):($5) linecolor rgb '#e5f5f9' pointsize 0.5 notitle,\
"filtered_cluster_cp.txt" using (2.8):($8) linecolor rgb '#2ca25f' pointsize 0.5 notitle,\
"filtered_cluster_ff.txt" using (3.2):($8) linecolor rgb '#e5f5f9' pointsize 0.5 notitle;

set xrange [3.5:6.5]
set xlabel "Memory Resources" font "Helvetica,16"
set xtics ("All" 1, "CPU" 2, "All Memories" 3, "Local" 4, "Local Remote" 5, "Remote Remote" 6) font "Helvetica, 12" 

set terminal pdf
set datafile separator ","
set output 'util-detailed-mem.pdf'
plot "filtered_cluster_cp.txt" using (3.8):($10*100/$9) linecolor rgb '#2ca25f' pointsize 0.5 title sprintf("CP - %i/%i Clusters", N_CP_NONE_ZERO,N_CP),\
"filtered_cluster_ff.txt" using (4.2):($10*100/$9) linecolor rgb '#e5f5f9' pointsize 0.5 title sprintf("FF - %i/%i Clusters", N_FF_NONE_ZERO,N_FF),\
"filtered_cluster_cp.txt" using (4.8):($13*100/$12) linecolor rgb '#2ca25f' pointsize 0.5 notitle,\
"filtered_cluster_ff.txt" using (5.2):($13*100/$12) linecolor rgb '#e5f5f9' pointsize 0.5 notitle,\
"filtered_cluster_cp.txt" using (5.8):($16*100/$15) linecolor rgb '#2ca25f' pointsize 0.5 notitle,\
"filtered_cluster_ff.txt" using (6.2):($16*100/$15) linecolor rgb '#e5f5f9' pointsize 0.5 notitle;



#Violations (Bar)

TOTAL_SLO_VIOLATION_FF = (system("awk -F, '{if($11 !=0 || $12 !=0){print $0}}' logicalServer-data.csv.ff | wc -l")-1)
TOTAL_SLO_VIOLATION_CP = (system("awk -F, '{if($11 !=0 || $12 !=0){print $0}}' logicalServer-data.csv.cp | wc -l")-1)
N_SLO_TOTAL = (system("cat logicalServer-data.csv.cp | wc -l")-1)

LOCAL_SLO_VIOLATION_CP = (system("awk -F, '{if($11 !=0){print $0}}' logicalServer-data.csv.cp | wc -l")-1)
LOCAL_SLO_VIOLATION_FF = (system("awk -F, '{if($11 !=0){print $0}}' logicalServer-data.csv.ff | wc -l")-1)

REMOTE_SLO_VIOLATION_CP = (system("awk -F, '{if($12 !=0){print $0}}' logicalServer-data.csv.cp | wc -l")-1)
REMOTE_SLO_VIOLATION_FF = (system("awk -F, '{if($12 !=0){print $0}}' logicalServer-data.csv.ff | wc -l")-1)

set style data histogram
set style histogram cluster gap 1

set xtics ("Total" 1, "Local" 2, "Remote" 3) font "Helvetica, 12" 
set xlabel "SLO Type" font "Helvetica,16"
set ylabel "SLO Violations (%)\n{/*0.75 1000 Logical Servers}" font "Helvetica,16"

set yrange [0:105]
set xrange [0:3.5]

set output 'violation-both.pdf'

plot '' using (0.8):(TOTAL_SLO_VIOLATION_CP*100/N_SLO_TOTAL) with boxes linecolor rgb '#2ca25f' title "CP",\
'' using (1.2):(TOTAL_SLO_VIOLATION_FF*100/N_SLO_TOTAL) with boxes linecolor rgb '#e5f5f9' title "FF",\
'' using (1.8):(LOCAL_SLO_VIOLATION_CP*100/N_SLO_TOTAL) with boxes linecolor rgb '#2ca25f' notitle,\
'' using (2.2):(LOCAL_SLO_VIOLATION_FF*100/N_SLO_TOTAL) with boxes linecolor rgb '#e5f5f9' notitle,\
'' using (2.8):(REMOTE_SLO_VIOLATION_CP*100/N_SLO_TOTAL) with boxes linecolor rgb '#2ca25f' notitle,\
'' using (3.2):(REMOTE_SLO_VIOLATION_FF*100/N_SLO_TOTAL) with boxes linecolor rgb '#e5f5f9' notitle;

set label 1 sprintf("%i", TOTAL_SLO_VIOLATION_FF) at 0.94,95 font "Helvetica, 10"
set label 2 sprintf("%i", LOCAL_SLO_VIOLATION_FF) at 1.94,95 font "Helvetica, 10"
set label 3 sprintf("%i", REMOTE_SLO_VIOLATION_FF) at 2.94,73 font "Helvetica, 10"
set output 'violation-ff.pdf'
plot '' using (1):(TOTAL_SLO_VIOLATION_FF*100/N_SLO_TOTAL) with boxes linecolor rgb '#2ca25f' notitle,\
'' using (2):(LOCAL_SLO_VIOLATION_FF*100/N_SLO_TOTAL) with boxes linecolor rgb '#2ca25f' notitle,\
'' using (3.):(REMOTE_SLO_VIOLATION_FF*100/N_SLO_TOTAL) with boxes linecolor rgb '#2ca25f' notitle;


#Price
OVERPRICES_FF_ALL = (system("awk -F, '{if($3>0 && NR!=1){print $0}}' price_compare.txt | wc -l"))
N_ALL = (system("awk -F, '{if(NR!=1){print $0}}' price_compare.txt | wc -l"))
MIN_OVERPRICE_FF_ALL = (system("awk -F, '{if($3>0 && NR!=1 ){print $3*100/$1}}' price_compare.txt | sort -g | head -n 1"))
MAX_OVERPRICE_FF_ALL = (system("awk -F, '{if($3>0 && NR!=1 ){print $3*100/$1}}' price_compare.txt | sort -g | tail -n 1"))
OVERPRICES_FF_SLO_APPROVED = (system("awk -F, '{if($3>0 && NR!=1 && $4==0 && $5==0){print $0}}' price_compare.txt | wc -l"))
N_SLA_APPROVED = (system("awk -F, '{if(NR!=1 && $4==0 && $5==0){print $0}}' price_compare.txt | wc -l"))
MIN_OVERPRICE_FF_SLO_APPROVED = (system("awk -F, '{if($3>0 && NR!=1 && $4==0 && $5==0){print $3*100/$1}}' price_compare.txt | sort -g | head -n 1"))
MAX_OVERPRICE_FF_SLO_APPROVED = (system("awk -F, '{if($3>0 && NR!=1 && $4==0 && $5==0){print $3*100/$1}}' price_compare.txt | sort -g | tail -n 1"))

set style data histogram
set style histogram cluster gap 1

set xtics ("All" 1, "Only Non-violated" 2) font "Helvetica, 12" 
set xlabel "Scenarios" font "Helvetica,16"
set ylabel "Overpriced Logical Servers (%)\n{/*0.75 1000 Logical Servers}" font "Helvetica,16"

set yrange [0:105]
set xrange [0:3]
unset label 1
unset label 2
unset label 3

set label 4 sprintf("%s%% to %s%%", MIN_OVERPRICE_FF_ALL, MAX_OVERPRICE_FF_ALL) at 0.1,40 font "Helvetica, 10"
set label 5 sprintf("%s%% to %s%%", MIN_OVERPRICE_FF_SLO_APPROVED, MAX_OVERPRICE_FF_SLO_APPROVED) at 2.2,60 font "Helvetica, 10"
set label 6 "Overpriced" at 2.38,55 font "Helvetica, 10"
set label 7 "Overpriced" at 0.32,35 font "Helvetica, 10"
set output 'price.pdf'
plot '' using (1):(OVERPRICES_FF_ALL*100/N_ALL) with boxes linecolor rgb '#2ca25f' notitle,\
'' using (2):(OVERPRICES_FF_SLO_APPROVED*100/N_SLA_APPROVED) with boxes linecolor rgb '#2ca25f' notitle;
