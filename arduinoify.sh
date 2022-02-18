echo -e "Making \e[32m\"hermes/\"\e[0m"
mkdir -p hermes/

echo -e "Moving \e[32m\"main.cpp\" \e[0m-> \e[32m\"hermes/hermes.ino\"\e[0m"
cp ./main.cpp hermes/hermes.ino

echo -e "Done!"
