# Définir l'image de base
FROM python:3.12.2-slim

# Définir le répertoire de travail dans le conteneur
WORKDIR /

# Copier le contenu du répertoire actuel dans le conteneur
COPY main.py /
COPY insertion_donnees.py /
COPY requirements.txt /

# Install dependencies
RUN pip install -r requirements.txt

# Set environment variables
ENV DB_HOST=database
ENV DB_PORT=5432
ENV DB_NAME=urbanfarm
ENV DB_USER=admin
ENV DB_PASSWORD=urbanfarm123
ENV API_PATH=/receive
ENV API_HOST=0.0.0.0
ENV API_PORT=8080

# Exposer le port 8080
EXPOSE 8080

# Commande pour exécuter l'application
CMD [ "python", "main.py"]
