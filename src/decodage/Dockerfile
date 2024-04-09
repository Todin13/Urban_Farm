# Utiliser une image de base avec g++ intégré
FROM gcc:latest

# Installer les dépendances nécessaires (cmake, msgpack et boost)
RUN apt-get update && apt-get install -y cmake libmsgpack-dev libboost-system-dev libboost-filesystem-dev

# Copier les éléments requis dans le conteneur
COPY . /usr/src/decodage

# Définir le répertoire de travail
WORKDIR /usr/src/decodage

# Compiler le programme
RUN g++ -o decodage decodage.cpp -lmsgpackc -lboost_system -lboost_filesystem

# Exécuter le programme
CMD ["./decodage"]

