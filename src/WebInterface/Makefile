DOCKER_REPO = todin13/
IMAGE_NAME = web-interface
TAG = 0.1.0
CONTAINER_NAME = Webinterface

prepare:
	@echo "Preparing the set up"
	poetry config virtualenvs.prefer-active-python true
	poetry config virtualenvs.in-project true
	poetry install --no-root

run:
	@echo "Starting the app"
	poetry run streamlit run main.py

check:
	@echo "Running Black"
	poetry run black --check .
	@echo "Running isort"
	poetry run isort --check .
	@echo "Running mypy"
	poetry run mypy .
	@echo "Running Vulture"
	poetry run vulture main.py
	@echo ""
	@echo "All goods !!!"

build:
	docker build -t $(DOCKER_REPO)$(IMAGE_NAME):$(TAG) .

push:
	docker push $(DOCKER_REPO)$(IMAGE_NAME):$(TAG)

deploy:
	docker run -p 8501:8501 --name $(CONTAINER_NAME) $(DOCKER_REPO)$(IMAGE_NAME):$(TAG)
