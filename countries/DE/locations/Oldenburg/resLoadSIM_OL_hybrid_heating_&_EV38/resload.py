import os
import shutil
import json
import subprocess

# Function to update the seed in the JSON configuration
def update_seed(seed=42):
    with open("./resLoadSIM.json", "r", encoding="utf-8") as f:
        config = json.load(f)
    config["seed"] = seed
    with open("./resLoadSIM.json", "w", encoding="utf-8") as f:
        json.dump(config, f, indent=2)

# Function to run the resLoadSIM tool
def run_resLoadSIM(num_households, num_days):
    subprocess.run([
        "/home/zaba_al/resLoadSIM-6.2.0-Source/build/resLoadSIM", 
        str(num_households), 
        str(num_days)
    ], cwd="./input")

# Function to copy results to a folder
def copy_results_to_folder():
    path = './output'
    os.makedirs(path, exist_ok=True)
    filelist = [f for f in os.listdir("./input/") if os.path.isfile(os.path.join("./input/", f))]
    for file in filelist:
        shutil.copy(f"./input/{file}", path)

# Main execution block
num_households = 93010   
num_days = 365
seed = 42

update_seed(seed)
run_resLoadSIM(num_households, num_days)
copy_results_to_folder()
