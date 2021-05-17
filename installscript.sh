sudo apt-get update
sudo apt-get -y install mysql-server
sudo apt-get -y install mysql-client
sudo apt-get -y install mysql-common
sudo apt-get -y install libmysqlclient20 
sudo apt-get -y install libmysqlclient-dev 
sudo apt-get -y install glade
sudo apt-get -y install ntp
sudo apt-get -y install gtk3.0
sudo apt-get -y install build-essential libgtk3.0-dev


cd DBServer
mysql -uroot -p <"Db.sql"
make
make install

cd ../DPDC
sudo make install

cd ../PMUSimulator
sudo make install


echo "INSTALLATION FINISHED"