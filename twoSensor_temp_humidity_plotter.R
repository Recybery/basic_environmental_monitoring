library("tidyr")
library("lubridate")
library("dplyr")
library("ggplot2")
library("readr")


compare_DHT11_test <- read_delim("~/Carrboro_Recybery/kode/environmental_monitoring/compare_DHT11.test.munged.dat", "\t", escape_double = FALSE, col_names = FALSE, trim_ws = TRUE)
compare_DHT11_test <- data.frame(compare_DHT11_test)
names(compare_DHT11_test) <- c('date', 'time', 'sensor', 'replicate', 'tempC', 'relHum','burst')

compare_DHT11_test$date<- ymd(compare_DHT11_test$date)
compare_DHT11_test$time<- hms(compare_DHT11_test$time)
compare_DHT11_test$dateTime <- compare_DHT11_test$date + compare_DHT11_test$time
compare_DHT11_test$dateTime<- as.POSIXct(compare_DHT11_test$dateTime)
compare_DHT11_test$replicate <- as.factor(compare_DHT11_test$replicate)
compare_DHT11_test$sensor <- as.factor(compare_DHT11_test$sensor)

#compare_DHT11_test.summarized <- compare_DHT11_test %>% group_by(burst) %>% summarize(meanTime = mean(dateTime), meanTempC = mean(tempC), sdTempC=sd(tempC), meanRelHum=mean(relHum), sdRelHum = sd(relHum))
compare_DHT11_test.summarized <- compare_DHT11_test %>% group_by(burst, sensor) %>% summarize(meanTime = mean(dateTime), meanTempC = mean(tempC), meanRelHum=mean(relHum))
#compare_DHT11_test.summarized <- compare_DHT11_test.summarized[ , !(names(compare_DHT11_test.summarized) %in% c("sdTempC"))]

compare_DHT11_test.summarized.gathered <- compare_DHT11_test.summarized %>%  gather(Measure,Value, meanTempC:meanRelHum)
compare_DHT11_test.summarized.gathered$Measure <- as.factor(compare_DHT11_test.summarized.gathered$Measure)



ggplot(compare_DHT11_test.summarized.gathered) + geom_line(aes(x=meanTime, y=Value, color=sensor)) + facet_grid(Measure~., scale="free") + geom_smooth(aes(x=meanTime, y=Value, group=sensor), color='black', size=2) +geom_smooth(aes(x=meanTime, y=Value, group=sensor, color=sensor)) + ggtitle("DHT11 Trace")
ggplot(compare_DHT11_test.summarized) + geom_point(aes(x=meanTempC, y=meanRelHum, color=meanTime))


