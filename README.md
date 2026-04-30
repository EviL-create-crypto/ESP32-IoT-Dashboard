Ce projet consiste à réaliser un système IoT avec une carte ESP32 capable de communiquer avec un serveur web pour envoyer, stocker et afficher des données en temps réel. L’ESP32 se connecte au WiFi, génère des valeurs et les transmet au serveur via des requêtes HTTP. Côté serveur, développé en PHP, les données sont enregistrées dans des fichiers afin de conserver la dernière valeur ainsi qu’un historique horodaté. Une interface web permet d’afficher ces informations dynamiquement grâce à AJAX, sans rechargement de la page. Le système permet également de contrôler l’ESP32 à distance, notamment pour piloter une LED ou un buzzer via des boutons sur la page web. La réalisation a nécessité la configuration du serveur Apache, la gestion des droits d’écriture et la mise en place de la communication entre les différents éléments. Ce projet m’a permis de comprendre le fonctionnement global d’un système connecté, incluant l’envoi de données, leur traitement côté serveur et le contrôle d’actionneurs à distance, constituant ainsi une base solide pour des applications IoT plus avancées.

Pour commencer, nous allons nous servir de fille zilla comme pont entre la machine physique et la VM (ubuntu) . Nous auron besoin de 4 programme , **1er est le code arduino pour ESP 32** sur la machine physique , et **les 3 autres sur la VM (data.php/valeur.csv/valeur.txt)**

Pour *le code arduino pour ESP 32* nous allons de part cela , changer les (**NomRéseau,Motdepasse ainsi que IPDELAVM/DOSSIERVOULU/PAGE.PHP**)

Pour *Le code data.php* , on vas changer l'IP et ainsi inserer celui de L'**ESP 32**.

🔵Pour valeur.csv et .txt vous aurai seulement besoin de les mettre dans un dossier commun sur votre VM. 
