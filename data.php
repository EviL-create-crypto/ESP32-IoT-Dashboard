<?php
// Définit le fuseau horaire (important pour avoir la bonne heure)
date_default_timezone_set("Pacific/Noumea");

// Nom des fichiers de stockage
$fichierTxt = "valeur.txt"; // stocke la dernière valeur
$fichierCsv = "valeur.csv"; // stocke l'historique

/* RECEPTION ESP32  */
// Si l'ESP32 envoie une valeur en POST
if (isset($_POST['valeur'])) {

    // Récupération de la valeur envoyée
    $valeur = $_POST['valeur'];

    // Récupération de la date et heure actuelles
    $date = date("Y-m-d");
    $heure = date("H:i:s");

    // 🔹 Enregistrement dans un fichier TXT (écrase l'ancienne valeur)
    file_put_contents($fichierTxt, $valeur);

    // 🔹 Enregistrement dans un fichier CSV (ajoute une ligne)
    $f = fopen($fichierCsv, "a"); // "a" = mode ajout
    fputcsv($f, [$date, $heure, $valeur]); // écrit une ligne CSV
    fclose($f); // ferme le fichier

    // Réponse envoyée à l'ESP32
    echo "OK";

    // Stop le script ici (important pour ne pas envoyer le HTML)
    exit;
}

/* LECTURE  */
// Fonction pour lire la dernière valeur
function getValue($file) {

    // Si le fichier existe
    if (file_exists($file)) {

        // Lit le contenu du fichier
        return ["valeur" => file_get_contents($file)];
    }

    // Si rien n'existe, retourne 0
    return ["valeur" => 0];
}

/*  AJAX */
// Si la page est appelée avec ?ajax=1
if (isset($_GET['ajax'])) {

    // Indique que la réponse est du JSON
    header("Content-Type: application/json");

    // Envoie la valeur sous forme JSON
    echo json_encode(getValue($fichierTxt));

    // Stop le script pour ne pas envoyer le HTML
    exit;
}
?>

<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<title>ESP32 Dashboard</title>

<style>

/* Style général de la page */
body {
    font-family: Arial;
    background: #0f172a;
    color: white;
    display: flex;
    justify-content: center;
    padding-top: 50px;
}

/* Boîte principale */
.container {
    width: 500px;
    background: #1e293b;
    padding: 20px;
    border-radius: 12px;
}

/* Boutons */
button {
    padding: 12px;
    margin: 5px;
    border-radius: 12px;
    font-size: 18px;
    cursor: pointer;
}

/* Zone d'affichage */
.box {
    margin-top: 15px;
    padding: 10px;
    background: #111827;
    border-radius: 10px;
}

</style>
</head>

<body>

<div class="container">

<h1>ESP32 Dashboard</h1>

<!-- Bouton pour allumer la LED -->
<button onclick="fetch('http://IPdelesp32/led')">
    LED
</button>

<!-- Bouton pour activer le buzzer -->
<button onclick="fetch('http://IPdelesp32/son')">
    BUZZER
</button>

<!-- Affichage de la valeur actuelle -->
<div class="box">
Valeur actuelle : <span id="valeur">0</span>
</div>

</div>

<script>

// Fonction pour récupérer la valeur depuis PHP
async function update() {

    // Appel du fichier PHP en mode AJAX
    const res = await fetch('data.php?ajax=1');

    // Conversion de la réponse en JSON
    const data = await res.json();

    // Mise à jour du texte dans la page
    document.getElementById('valeur').textContent = data.valeur;
}

// Mise à jour automatique toutes les 2 secondes
setInterval(update, 2000);

// Appel immédiat au chargement
update();

</script>

</body>
</html>