# Définir l'image de base
FROM python:3.12.2

# Définir le répertoire de travail dans le conteneur
WORKDIR /

# Copier le contenu du répertoire actuel dans le conteneur
COPY . /

RUN pip install poetry && poetry install --no-root

# Considérer le port 8080
EXPOSE 8080

# Exécuter le script Python lorsque le conteneur se lance
#CMD ["poetry", "run", "python", "main.py"]

CMD ["poetry", "run", "python", "main.py"]
