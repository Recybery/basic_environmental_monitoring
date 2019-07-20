library("tidyr")
library("lubridate")
library("dplyr")
library("ggplot2")
library("readr")

//setwd("/home/recybery/Projects/environmental_monitoring_dev/")

timeLapse <- read_delim("/home/recybery/Projects/environmental_monitoring_dev/theLogJam/LOGGERJuly7.munged.tsv", "\t", escape_double = FALSE, col_names = TRUE, trim_ws = TRUE, guess_max = 10000000) %>% as_tibble() 
timeLapse <- timeLapse %>% separate(col = timestamp, into = c("date", "time"), sep = "-") %>% mutate(date=dmy(date), time=hms(time), dateTime = as.POSIXct( date + time))
timeLapse$rep <- as.factor(timeLapse$rep)

timeLapse.summarized <- timeLapse %>% group_by(burst) %>% summarize(meanTime = mean(dateTime), meanTempA = mean(tempA), stdTempA=sd(tempA), meanTempB = mean(tempB), stdTempB=sd(tempB), meanRelHumA=mean(relhumA), stdRelHumA = sd(relhumA), meanRelHumB=mean(relhumB), stdRelHumB = sd(relhumB), meanWt = mean(weight), stdWt = sd(weight), meanLux = mean(lumen), stdLux = sd(lumen) )

raw.gg <- ggplot(timeLapse.summarized) + geom_line(aes(x=meanTime, y = meanWt)) + labs(x = "calendar time", y= "recorded weight (kg)", title="Smoothed Recorded Weight for Medium-Term Stability Test" ) + theme_bw()
png(filename="raw_average_scale_trace.png", height = 500, width = 750, res=100)
raw.gg
dev.off()

timeLapse.summarized %>% filter(meanWt >5) %>% select(meanWt) %>% nrow()
timeLapse.summarized %>% filter(meanWt >5) %>% select(meanWt) %>% summary()

inner_join(timeLapse, timeLapse.summarized %>% filter(meanWt >5) %>% select(burst)) 
inner_join(timeLapse, timeLapse.summarized %>% filter(meanWt >5) %>% select(burst)) %>% select(time) %>% summary()

weightByBurst.gg<- ggplot(inner_join(timeLapse, timeLapse.summarized %>% filter(meanWt >5) %>% select(burst)) %>% mutate(burst=as.factor(burst)) ) + geom_jitter(aes(x=rep, y=weight, color=dateTime),height=0, width = 0.25) +theme_bw() + labs(x = "burst replicate", y="recorded weight (kg)", title="Recorded Weight by Burst Replicate", color = "calendar time")

png(filename="weight_by_burst.png", height = 500, width = 750, res=100)
weightByBurst.gg
dev.off()

filteredWt.gg<- ggplot()  + geom_point(data=timeLapse %>% filter(weight < 5 & weight >0) %>% mutate(avg_temp = (tempA+tempB)/2 ), aes(x=dateTime, y = weight, color=avg_temp)) + scale_color_gradient(low="blue", high="red") + geom_line(data = timeLapse.summarized %>% filter(meanWt < 5) %>% filter(meanWt >0), aes(x=meanTime, y=meanWt )) + labs(x = "calendar time", y="weight (kg)" , color = "temperature\n(deg. C)", title="Filtered, Averaged Weight Measurements,\n with Raw Measurements Colored by Temperature") + theme_bw()

png(filename="FilteredWeightTimeSeries.png", height = 500, width = 750, res=100)
filteredWt.gg
dev.off()

sensorDiscrep <- timeLapse.summarized %>%  mutate(deltaT = meanTempB-meanTempA, deltaH = meanRelHumB - meanRelHumA) %>% select(burst, meanTime, deltaT, deltaH ) 
sensorDiscrep.gath <- sensorDiscrep %>%  gather(key = measure, value = value , c(deltaT, deltaH) )  %>%  mutate(measure=as.factor(measure)) 
levels(sensorDiscrep.gath$measure) <- c("deg. C","% rel. hum.")
discrep.gg <- ggplot(sensorDiscrep.gath %>% filter(!(measure == "deg. C" & value < -5)))  + geom_line(aes(x=meanTime, y=value, color = measure)) + facet_grid(measure~., scales = "free_y") + labs(x = "calendar time", y = "discrepancy", title = "Systematic Difference Between Two DHT11 Temperature/Humidity Sensors") + theme_bw() + theme(legend.position = "none")

png(filename="TempHumDiscrepTimeSeries.png", height = 500, width = 750, res=100)
discrep.gg
dev.off()

