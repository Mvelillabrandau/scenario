#!/usr/bin/env Rscript
# Copyright (c) 2012-2015  Alexander Afanasyev <alexander.afanasyev@ucla.edu>


# install.packages ('ggplot2')
library(ggplot2)
## # install.packages ('scales')
## library (scales)

#########################
# Rate trace processing #
#########################
data = read.table("../results/FIFO/drop-trace-fifo-T2.txt", header=T)
data$Node = factor(data$Node)
data$Kilobits <- data$Kilobytes * 8
data$Type = factor(data$Type)

## data.rtr = data[grep("Rtr", data$Node),]

# graph rates on all nodes in Kilobits
g.all <- ggplot(data, aes(x=Time, y=Kilobits, color=Type)) +
  geom_point(size=2) +
  geom_line() +
  ylab("Packet drop rate [Kbits/s]") +
  facet_wrap(~ Node) +
  theme_bw()

png("../results/FIFO/l2-rate-tracer-fifo-T2.png", width=800, height=500)
print(g.all)
x = dev.off()
