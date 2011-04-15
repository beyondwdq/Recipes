set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
#set xtic auto                          # set xtics automatically
#set ytic auto                          # set ytics automatically
set size 1,1
set xlabel "Time"
set ylabel "Online user count"
#set key 280,20
#set term post eps color enh
set term jpeg medium
set out "online_user_cnt.jpeg"

plot 'online_user_cnt.txt' using 1:2  notitle with linespoints
