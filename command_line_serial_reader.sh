#sudo apt-get install moreutils ttylog
cereal=$1
#e.g., /dev/ttyACM1
phial_out=$2

ttylog -b 9600 -d "$cereal" -f |  ts '%Y-%m-%d	%H:%M:%S' | tee "$phial_out"
