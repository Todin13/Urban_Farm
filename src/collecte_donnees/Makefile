DOCKER_REPO = todin13/
IMAGE_NAME = api-data-collection
TAG = 0.1.0
CONTAINER_NAME = data-collect

build:
	docker build -t $(DOCKER_REPO)$(IMAGE_NAME):$(TAG) .

push:
	docker push $(DOCKER_REPO)$(IMAGE_NAME):$(TAG)
