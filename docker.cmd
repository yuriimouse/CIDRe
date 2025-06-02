@REM runs Docker under Windows
cd ./docker
docker compose up -d
docker compose exec cidre-lib /bin/bash
cd ..
