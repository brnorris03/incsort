#
#	sort_times.R
#
#	plot the results from incsort
#

#
#	Benjamin C. Haller, 4/11/2020
#


# this data should be copy-pasted from the output of incsort
iter_count <- c(25)
fetch_counts <- c(1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608)
pre_times <- c(13294849, 13301705, 13291840, 13282563, 13284649, 13290572, 13292710, 13277278, 13283910, 13285369, 13283627, 13282132, 13291799, 13285412, 13280582, 13289327, 13284819, 13286665, 13296321, 13310699, 13334015, 13383439, 13461732, 13666016)
partial_times <- c(135432, 135510, 136848, 135829, 135735, 135382, 134792, 273827, 271751, 414232, 569975, 732175, 939545, 1204601, 1602192, 2219418, 3287611, 5148100, 8381462, 13689609, 22029716, 34670863, 48532117, 52247608)
iqs_times <- c(1089673, 956824, 828381, 991452, 879590, 861456, 1023050, 982575, 724232, 925681, 953745, 912002, 1033636, 906111, 1044265, 862176, 1086575, 1090294, 1301649, 1876279, 2807354, 4652283, 8319537, 14939153)

# average across the number of iterations
pre_times <- pre_times / iter_count
partial_times <- partial_times / iter_count
iqs_times <- iqs_times / iter_count

# convert microseconds to milliseconds
pre_times <- pre_times / 1000
partial_times <- partial_times / 1000
iqs_times <- iqs_times / 1000


# plot
quartz(width=6, height=4.5, type="pdf", file="~/Desktop/sort_times.pdf")
par(mar=c(3.5, 3.5, 0.5, 0.5), family="serif", mgp=c(2.1, 0.6, 0), cex.axis=0.8)
plot(range(fetch_counts), range(c(pre_times, partial_times, iqs_times)), type="n", log="xy", xlab="Elements requested", ylab="Time (ms)", xaxt="n")
axis(1, at=c(1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7), labels=c("1", "10", "10^2", "10^3", "10^4", "10^5", "10^6", "10^7"))
abline(v=c(1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7), col="lightgray")
lines(fetch_counts, pre_times, col="black", lwd=2)
points(fetch_counts, pre_times, col="black", pch=19, cex=0.7)
lines(fetch_counts, partial_times, col="cornflowerblue", lwd=2)
points(fetch_counts, partial_times, col="cornflowerblue", pch=19, cex=0.7)
lines(fetch_counts, iqs_times, col="chartreuse3", lwd=2)
points(fetch_counts, iqs_times, col="chartreuse3", pch=19, cex=0.7)
legend("bottomright", legend=c("pre-sort", "partial sort", "incremental quicksort"), cex=0.8, col=c("black", "cornflowerblue", "chartreuse3"), lwd=2, bg="white", inset=0.02)
box()
dev.off()















