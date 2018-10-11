
library("readr")
library("lubridate")
library("dplyr")
library("ggplot2")



threePointLumin_tempHum <- read_delim("threePointLumin_tempHum.munged.dat", "\t", escape_double = FALSE, col_names = FALSE, trim_ws = TRUE)

threePointLumin_tempHum <- data.frame(threePointLumin_tempHum)
names(threePointLumin_tempHum) <- c('date', 'time', 'burst', 'replicate', 'tempC', 'relHum', 'lumin_L', 'lumin_R', 'lumin_M')

threePointLumin_tempHum$date<- ymd(threePointLumin_tempHum$date)
threePointLumin_tempHum$time<- hms(threePointLumin_tempHum$time)
threePointLumin_tempHum$dateTime <- threePointLumin_tempHum$date + threePointLumin_tempHum$time
threePointLumin_tempHum$dateTime<- as.POSIXct(threePointLumin_tempHum$dateTime)
threePointLumin_tempHum$replicate <- as.factor(threePointLumin_tempHum$replicate)
#threePointLumin_tempHum$sensor <- as.factor(threePointLumin_tempHum$sensor)

threePointLumin_tempHum.summarized <- threePointLumin_tempHum %>% group_by(burst) %>% summarize(meanTime = mean(dateTime), meanTempC = mean(tempC), sdTempC=sd(tempC), meanRelHum=mean(relHum), sdRelHum = sd(relHum), meanLumL=mean(lumin_L), sdLumL = sd(lumin_L), meanLumR=mean(lumin_R), sdLumR = sd(lumin_R), meanLumM=mean(lumin_M), sdLumM = sd(lumin_M))

start <- as.POSIXct(ymd("2018-02-11")+hms("00:00:01"))
stop <- as.POSIXct(ymd("2018-02-12")+hms("20:00:00"))


#ggplot(threePointLumin_tempHum.summarized) + geom_line(aes(x=meanTime, y=meanLumL), color='red', alpha=0.5) + geom_line(aes(x=meanTime, y=meanLumR), color='blue') + geom_line(aes(x=meanTime, y=meanLumM), color='green') 
#ggplot(threePointLumin_tempHum.summarized) + geom_line(aes(x=meanTime, y=sdLumL), color='red', alpha=0.5) + geom_line(aes(x=meanTime, y=sdLumR), color='blue') + geom_line(aes(x=meanTime, y=sdLumM), color='green')
#ggplot(threePointLumin_tempHum.summarized) + geom_line(aes(x=meanTime, y=sdLumL/meanLumL), color='red', alpha=0.5) + geom_line(aes(x=meanTime, y=sdLumR/meanLumR), color='blue') + geom_line(aes(x=meanTime, y=sdLumM/meanLumM), color='green')  

#ggplot(threePointLumin_tempHum.summarized %>% filter(meanTime > start) %>% filter(meanTime < stop))  + geom_line(aes(x=meanTime, y=meanLumL), color='red', alpha=0.5) + geom_smooth(aes(x=meanTime, y=meanLumL), color='black')
threePointLumin_tempHum.summarized$barTop <- 1000

png(filename="luminosity_temp_timeline.png", width=1000, height=600)
ggplot(threePointLumin_tempHum.summarized %>% filter(meanTime > start) %>% filter(meanTime < stop)) + geom_col(aes(x=meanTime, y=barTop, color=meanTempC)) + geom_line(aes(x=meanTime, y=meanLumL), color='red', alpha=0.5) + geom_smooth(aes(x=meanTime, y=meanLumL), color='black') + labs(x="Calendar Time", y="Sensor Response", title="Two Days of Luminosity and Temperature Readings") + scale_color_continuous(name='Ambient\nTemp (C)') + theme_minimal()
dev.off()


