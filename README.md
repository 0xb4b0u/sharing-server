# Serveur de partage en C

## Table des matières 📚

- [Serveur de partage en C](#serveur-de-partage-en-c)
  - [Table des matières](#table-des-matières)
  - [Prérequis 🛠️](#prérequis)
  - [Fonctionnalités ✨](#fonctionnalités)
  - [Installation et Utilisation ⚙️](#installation-et-utilisation)
  - [Améliorations possibles 📋](#améliorations-possibles)
  - [Auteurs ✍️](#auteurs)
  - [License 📝](#license)

---

## Prérequis 🛠️

Afin d'exécuter ce projet, vous aurez besoin des outils suivants installés sur votre machine :
 - [Make](https://www.gnu.org/software/make/) : un outil qui contrôle la génération de programmes exécutables et d'autres tâches non liées à la compilation de fichiers source. Il est généralement préinstallé sur la plupart des distributions Linux.
 - [CMake](https://cmake.org/) : un outil open-source qui contrôle le processus de génération de logiciels en utilisant des fichiers de configuration indépendants de la plate-forme et du compilateur.
 - [GCC](https://gcc.gnu.org/) : le compilateur GNU. Il est généralement préinstallé sur la plupart des distributions Linux.
 - [OpenSSL](https://www.openssl.org/) : une bibliothèque open-source qui implémente des protocoles de sécurité tels que SSL/TLS pour sécuriser les communications sur un réseau informatique.

---

## Fonctionnalités ✨

- **Sécurisation des communications** : Utilisation de SSL/TLS pour sécuriser les communications entre le client et le serveur.
- **Vérification d'intégrité** : Calcul et comparaison des hash MD5 pour vérifier l'intégrité des fichiers transférés.
- **Gestion des erreurs** : Gestion des erreurs de connexion, d'ouverture de fichier, etc.


---

## Installation et Utilisation ⚙️

Pour cloner ce projet sur votre machine locale, ouvrez un terminal et exécutez la commande suivante :

```bash
git clone 
cd bidule
```

1. Créez un répertoire build et naviguez dedans :
```bash
mkdir build
cd build
```

2. Générez les fichiers de build avec CMake :
```bash
cmake ..
```

3. Compilez le projet :
```bash
make
```
Cette commande génère deux exécutables : `client` et `server`.

### Serveur

Le serveur doit être lancé en premier. Il prend en paramètre une IP, un port ainsi que le chemin vers 
la liste contenant le nom des différents fichiers et leur hash correspondant.

```bash
./server -i <IP> -p <PORT> -h <hash_list_path>
```

### Client

Le client prend en paramètre une IP, un port et le nom du fichier à récupérer. Il se connecte au serveur et télécharge 
le fichier indiqué en paramètre. Ensuite, il calcule le hash de celui-ci et le compare avec le hash reçu du serveur.

```bash
./client -i <IP> -p <PORT> -f <file_name>
```


## Améliorations possibles 📋

- [ ] Support de plusieurs clients en simultané
- [ ] Gestion des erreurs
- [ ] Support pour les autres types de données (images, vidéos, etc.)

---

## Auteurs ✍️
- **Baptiste Oger** - *Travail initial* - [0xb4b0u](https://github.com/0xb4b0u)


## License 📝
Ce projet est sous licence MIT - voir le fichier [LICENSE.md](LICENSE.md) pour plus de détails
