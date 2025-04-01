import subprocess
import os
from dotenv import load_dotenv
from openai import OpenAI

# Cargar la API key desde .env
load_dotenv()
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

# Agregar todos los cambios
subprocess.run(["git", "add", "."], check=True)

# Obtener nombre de la rama actual
branch = subprocess.run(["git", "branch", "--show-current"], capture_output=True, text=True, encoding="utf-8").stdout.strip()

# Obtener el diff de los archivos preparados
diff = subprocess.run(["git", "diff", "--cached"], capture_output=True, text=True, encoding="utf-8").stdout

if not diff.strip():
    print("No hay cambios preparados para commit.")
    exit()

# Llamada a ChatGPT con la nueva API
response = client.chat.completions.create(
    model="gpt-4",
    messages=[
        {"role": "system", "content": "Eres un desarrollador profesional. Resume este diff en un mensaje de commit breve y claro, en español."},
        {"role": "user", "content": f"Diff en la rama {branch}:\n{diff}"}
    ]
)

mensaje = response.choices[0].message.content.strip()

print(f"\nMensaje sugerido:\n> {mensaje}\n")

confirmar = input("¿Confirmar commit y push? (s/n): ").lower()
if confirmar == 's':
    subprocess.run(["git", "commit", "-m", mensaje], check=True)
    subprocess.run(["git", "push", "origin", branch], check=True)
    print("✅ Commit y push realizados con éxito.")
else:
    print("❌ Commit cancelado.")