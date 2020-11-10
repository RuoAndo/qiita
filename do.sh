COUNT=10000

while [ $COUNT -lt 30000 ]; do

    ./random_data ${COUNT}
    ./ipaddress11 random_data.txt 
    
    COUNT=`expr $COUNT + 1000` # COUNT をインクリメント
done
