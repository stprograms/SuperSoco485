python -m venv .venv

source .venv/bin/activate
python -m pip install --upgrade pip
pip install --upgrade platformio

echo 'source .venv/bin/activate' >> ~/.bashrc