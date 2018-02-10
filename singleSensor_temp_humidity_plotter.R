
library("lubridate")
library("dplyr")
library("ggplot2")

single_sensor_test <- read_delim("~/Carrboro_Recybery/kode/environmental_monitoring/single_sensor.munged.dat", "\t", escape_double = FALSE, col_names = FALSE, trim_ws = TRUE)
single_sensor_test <- data.frame(single_sensor_test)
names(single_sensor_test) <- c('date', 'time', 'burst', 'replicate', 'tempC', 'relHum')

single_sensor_test$date<- ymd(single_sensor_test$date)
single_sensor_test$time<- hms(single_sensor_test$time)
single_sensor_test$dateTime <- single_sensor_test$date + single_sensor_test$time
single_sensor_test$dateTime<- as.POSIXct(single_sensor_test$dateTime)
single_sensor_test$replicate <- as.factor(single_sensor_test$replicate)
#single_sensor_test.summarized <- single_sensor_test %>% group_by(burst) %>% summarize(meanTime = mean(dateTime), meanTempC = mean(tempC), sdTempC=sd(tempC), meanRelHum=mean(relHum), sdRelHum = sd(relHum))
single_sensor_test.summarized <- single_sensor_test %>% group_by(burst) %>% summarize(meanTime = mean(dateTime), meanTempC = mean(tempC), meanRelHum=mean(relHum))
single_sensor_test.summarized.gathered <- single_sensor_test.summarized %>%  gather(Measure,Value, meanTempC:meanRelHum)
#single_sensor_test.summarized.gathered <- single_sensor_test.summarized %>%  gather(Measure,Value, meanTempC:sdRelHum)
single_sensor_test.summarized.gathered$Measure <- as.factor(single_sensor_test.summarized.gathered$Measure)

#https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/

png(filename="skipped_beat.png", width=1000, height=600)
ggplot(single_sensor_test.summarized.gathered %>% filter(meanTime < ymd_hms("2018-02-03 23:59:59")) %>% filter(meanTime > ymd_hms("2018-02-03 18:00:00")) ) + geom_line(aes(x=meanTime, y=Value)) + facet_grid(Measure~., scale="free") + ggtitle("DHT11 Trace") 
dev.off()


ggplot(single_sensor_test.summarized.gathered  ) + geom_line(aes(x=meanTime, y=Value)) + facet_grid(Measure~., scale="free") + ggtitle("DHT11 Trace") + geom_smooth(aes(x=meanTime, y=Value))
