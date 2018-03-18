library("tidyr")
library("lubridate")
library("dplyr")
library("ggplot2")
library("readr")

setwd("/home/mayko/Carrboro_Recybery/kode/basic_environmental_monitoring")
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

levels(compare_DHT11_test.summarized.gathered$Measure) <- c("Mean Relative Humidity (%)","Mean Temperature (C)")

png(filename="double_DHT11_timeseries.png", width=1000, height=600, res=100)
ggplot(compare_DHT11_test.summarized.gathered) + geom_line(aes(x=meanTime, y=Value, color=sensor)) + facet_grid(Measure~., scale="free") + geom_smooth(aes(x=meanTime, y=Value, group=sensor), color='black', size=2) +geom_smooth(aes(x=meanTime, y=Value, group=sensor, color=sensor)) + ggtitle("Two DHT11 Sensors - Temperature and Humidity Trace")
dev.off()



#ggplot(compare_DHT11_test.summarized) + geom_point(aes(x=meanTempC, y=meanRelHum, color=sensor))


#ggplot(compare_DHT11_test.summarized.gathered %>% filter(meanTime < ymd_hms("2018-02-06 12:00:00")) %>% filter(meanTime > ymd_hms("2018-02-05 00:12:00"))) + geom_line(aes(x=meanTime, y=Value, color=sensor)) + facet_grid(Measure~., scale="free") + geom_smooth(aes(x=meanTime, y=Value, group=sensor), color='black', size=2) +geom_smooth(aes(x=meanTime, y=Value, group=sensor, color=sensor)) + ggtitle("DHT11 Trace")


compare_DHT11_test.summarized.gathered.united <- compare_DHT11_test.summarized.gathered %>% unite(temp, Measure, sensor) %>% spread(temp, Value) %>% group_by(burst) %>% summarize(meanTime = mean(meanTime), meanRelHum_A = mean(meanRelHum_A, na.rm=TRUE), meanRelHum_B = mean(meanRelHum_B, na.rm=TRUE), meanTempC_A = mean(meanTempC_A, na.rm=TRUE), meanTempC_B = mean(meanTempC_B, na.rm=TRUE))
compare_DHT11_test.summarized.gathered.united$deltaHum <- compare_DHT11_test.summarized.gathered.united$meanRelHum_B - compare_DHT11_test.summarized.gathered.united$meanRelHum_A
compare_DHT11_test.summarized.gathered.united$deltaTemp <- compare_DHT11_test.summarized.gathered.united$meanTempC_B - compare_DHT11_test.summarized.gathered.united$meanTempC_A

compare_DHT11_test.summarized.gathered.united$meanHum <- (compare_DHT11_test.summarized.gathered.united$meanRelHum_B + compare_DHT11_test.summarized.gathered.united$meanRelHum_A)/2
compare_DHT11_test.summarized.gathered.united$meanTemp <- (compare_DHT11_test.summarized.gathered.united$meanTempC_B + compare_DHT11_test.summarized.gathered.united$meanTempC_A)/2

#ggplot(compare_DHT11_test.summarized.gathered.united) + geom_line(aes(x=meanTime, y=deltaHum, color=meanHum))
#ggplot(compare_DHT11_test.summarized.gathered.united) + geom_line(aes(x=meanTime, y=deltaTemp, color=meanTemp))




compare_DHT11_test.summarized.gathered.united.regathered <- compare_DHT11_test.summarized.gathered.united %>% select("meanTime","deltaHum","deltaTemp","meanHum","meanTemp") %>%  gather(measure, value, -meanTime)
compare_DHT11_test.summarized.gathered.united.regathered$measure <- as.factor(compare_DHT11_test.summarized.gathered.united.regathered$measure)
levels(compare_DHT11_test.summarized.gathered.united.regathered$measure) <- c("Δ Humidity (%)","Δ Temperature (C)", "Mean Humidity (%)","Mean Temperature (C)" )
compare_DHT11_test.summarized.gathered.united.regathered$hour <- as.factor(hour(compare_DHT11_test.summarized.gathered.united.regathered$meanTime))



#ggplot(compare_DHT11_test.summarized.gathered.united.regathered) + geom_line(aes(x=meanTime, y=value)) + facet_grid(measure~., scale='free')
#compare_DHT11_test.summarized.gathered.united$date <-  as.Date(compare_DHT11_test.summarized.gathered.united$meanTime, "%A, %B %d, %Y %X")
#sprintf("%i:%i:%i",month(t),day(t),hour(t))

png(filename="double_DHT11_timeseries_Delta.png", height = 1000, width = 600, res=100)
ggplot(compare_DHT11_test.summarized.gathered.united.regathered ) + geom_point(aes(x=meanTime, y=value, color=hour)) +  geom_line(aes(x=meanTime, y=value), color='black', alpha=0.75) + facet_grid(measure~., scale='free') + guides(col = guide_legend(nrow = 24, byrow = TRUE, title = "Clock Hour")) + xlab("Calendar Time") + ggtitle("Temperature and Humidity Measurements and Discrepancies for Two DHT11 Sensors")
dev.off()


png(filename="double_DHT11_timeseries_DeltaT_closeup.png", height = 600, width = 1000, res=100)
ggplot( compare_DHT11_test.summarized.gathered.united.regathered %>% mutate(measure = as.character(measure)) %>% filter( grepl("Temp",measure)) %>% mutate(measure = as.factor(measure)) %>% filter(meanTime < ymd_hms("2018-02-06 08:00:00")) %>% filter(meanTime > ymd_hms("2018-02-06 02:00:00"))) + geom_point(aes(x=meanTime, y=value, color=hour)) +  geom_line(aes(x=meanTime, y=value), color='black', alpha=0.75) + facet_grid(measure~., scale='free') + guides(col = guide_legend(nrow = 24, byrow = TRUE, title = "Clock Hour")) + xlab("Calendar Time") + ggtitle("Closeup Temperature Measurements and Discrepancies for Two DHT11 Sensors")
dev.off()


png(filename="double_DHT11_timeseries_DeltaH_closeup.png", height = 600, width = 1000, res=100)
ggplot( compare_DHT11_test.summarized.gathered.united.regathered %>% mutate(measure = as.character(measure)) %>% filter( grepl("Hum",measure)) %>% mutate(measure = as.factor(measure)) %>% filter(meanTime < ymd_hms("2018-02-05 18:00:00")) %>% filter(meanTime > ymd_hms("2018-02-05 8:00:00"))) + geom_point(aes(x=meanTime, y=value, color=hour)) +  geom_line(aes(x=meanTime, y=value), color='black', alpha=0.75) + facet_grid(measure~., scale='free') + guides(col = guide_legend(nrow = 24, byrow = TRUE, title = "Clock Hour")) + xlab("Calendar Time") + ggtitle("Closeup Humidity Measurements and Discrepancies for Two DHT11 Sensors")
dev.off()

