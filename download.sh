# copy repo from github to local, then get only server folder and delete the rest

cd ~/projects/esa-project/tmp
if [ -d "esa-project-2024" ]
then sudo rm -Rf esa-project-2024
fi
sudo git clone https://github.com/michaspie8/esa-project-2024.git
cd esa-project-2024/esp32
if [ -d "~/projects/esa-project/server" ]
then sudo rm -Rf ~/projects/esa-project/server
fi
sudo cp -r server ~/projects/esa-project
cd ~/projects/esa-project
sudo rm -rf tmp/esa-project-2024
echo "Download complete"