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
    model="gpt-4.1-mini",
    messages=[
        {"role": "system", "content": "Eres un desarrollador novato en Unreal Engine y C++. Resume este diff en un mensaje de commit completo: incluye primero una línea breve estilo Conventional Commit (por ejemplo: 'feat: ...' o 'fix: ...'), y luego una descripción detallada con viñetas o párrafos. El resultado será usado directamente en un commit Git en español."},
        {"role": "user", "content": f"Diff en la rama {branch}:\n{diff}"}
    ]
)


mensaje_completo = response.choices[0].message.content.strip()

#   limpiar bloques de código
if mensaje_completo.startswith("```"):
    mensaje_completo = mensaje_completo.strip("`")
    mensaje_completo = "\n".join(mensaje_completo.split("\n")[1:])
    
#   separar título y cuerpo
lineas = mensaje_completo.split("\n", 1)
titulo = lineas[0]
cuerpo = lineas[1] if len(lineas) > 1 else ""
bp_comment = input("¿Has hecho cambios en algún Blueprint? Describe brevemente (ENTER para omitir): ").strip()
if bp_comment:
    cuerpo += f"\n\n Cambios en Blueprint: \n- {bp_comment}"


print("\n====== MENSAJE DE COMMIT ======")
print (f"{titulo}\n")
print (cuerpo)
print("====================================\n")

confirmar = input("¿Confirmar commit y push? (s/n): ").lower()
if confirmar == 's':
    subprocess.run(["git", "commit", "-m", titulo, "-m", cuerpo], check=True)
    subprocess.run(["git", "push", "origin", branch], check=True)
    print("✅ Commit y push realizados con éxito.")
else:
    print("❌ Commit cancelado.")