#!/bin/bash

echo "Démarrage du projet..."

# change permission of python files in cgi-bin folder
echo "Changement des permissions des fichiers Python dans le dossier cgi-bin..."
chmod u+x ./server/cgi-bin/*.py

# start MQTT broker
echo "Démarrage du broker MQTT..."
sudo systemctl start mosquitto

# attendre que le broker MQTT soit lancé
sleep 2

# start server
echo "Démarrage du serveur Web..."
cd ./server
python server.py &

# attendre que le serveur Web soit lancé
while ! pgrep -f "python server.py" > /dev/null; do
    sleep 1
done
cd ..

# start MQTT client
echo "Démarrage du client MQTT..."
cd ./mqtt_client
python client_raspberry.py &

# attendre que le client MQTT soit lancé
while ! pgrep -f "python client_raspberry.py" > /dev/null; do
    sleep 1
done
cd ..

# attendre un peu avant de demander à lancer la page web
sleep 5

# demande à l'utilisateur s'il veut lancer la page Web des données
read -p "Voulez-vous ouvrir la page Web des données ? (y/n)" choice
case "$choice" in 
  y|Y ) 
    echo "Ouverture de la page Web des données..."
    xdg-open "http://localhost:8000"
    ;;
  n|N ) 
    echo "Vous avez choisi de ne pas ouvrir la page Web des données."
    ;;
  * ) 
    echo "Choix invalide. La page Web des données ne sera pas ouverte."
    ;;
esac

echo "Le projet a été démarré avec succès !"
