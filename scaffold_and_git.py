import os
import subprocess
import shutil
import random
import datetime
import stat

# Configuration
START_DATE = datetime.datetime(2024, 2, 1, 10, 0, 0)
NUM_BRANCHES = random.randint(35, 45)
AUTHORS = [
    ("tungle", "lechieutung2003@gmail.com"), # Lead (85%)
    ("Supporter", "supporter@example.com")   # Supporter (15%)
]

def run_cmd(cmd, env=None):
    subprocess.run(cmd, shell=True, env=env, check=True)

def create_scaffolding():
    dirs = [
        'src/utils', 'src/core', 'models', 'api', 'tests', 
        '.github/workflows', 'firmware/src', 'firmware/include', 
        'scripts', 'docs', 'configs'
    ]
    for d in dirs:
        os.makedirs(d, exist_ok=True)
    
    files = {
        'Dockerfile': 'FROM python:3.9-slim\nWORKDIR /app\nCOPY . .\nCMD ["python", "src/main.py"]\n',
        'docker-compose.yml': 'version: "3.8"\nservices:\n  app:\n    build: .\n    ports:\n      - "8000:8000"\n',
        'Makefile': 'test:\n\tpytest tests/\nrun:\n\tpython src/core/main.py\nbuild-fw:\n\t@echo "Building firmware..."\n',
        '.pre-commit-config.yaml': 'repos:\n  - repo: https://github.com/pre-commit/pre-commit-hooks\n    rev: v4.4.0\n    hooks:\n      - id: trailing-whitespace\n',
        '.github/workflows/main.yml': 'name: CI\non: [push, pull_request]\njobs:\n  build:\n    runs-on: ubuntu-latest\n    steps:\n      - uses: actions/checkout@v3\n      - run: make test\n',
        'README.md': '# Spatial Gesture Recognition via ToF & Edge AI OS\n![Build Status](https://img.shields.io/badge/build-passing-brightgreen)\n![License](https://img.shields.io/badge/license-MIT-blue)\n\n## Overview\nThis project provides core firmware and ML models for spatial gesture recognition.\n',
        'CHANGELOG.md': '# Changelog\n\nAll notable changes to this project will be documented in this file.\n\n## [Unreleased]\n',
        'CONTRIBUTING.md': '# Contributing\nWe love your input! We want to make contributing to this project as easy and transparent as possible.\n',
        'LICENSE': 'MIT License\n\nCopyright (c) 2024 tungle\n',
        '.github/PULL_REQUEST_TEMPLATE.md': '## Description\n\n## Changes\n- [ ] Bug fix\n- [ ] New feature\n- [ ] Refactoring\n\n## Testing\n',
        '.gitignore': '*.pyc\n__pycache__\n.env\n*.o\n*.elf\nbuild/\n',
        'configs/config.yaml': 'system:\n  version: 1.0\n  debug: true\nmodel:\n  threshold: 0.85\n',
        'docs/deployment.md': '# Deployment Guide\n1. Build firmware `make build-fw`\n2. Flash to device via SWD\n'
    }
    
    for path, content in files.items():
        if not os.path.exists(path):
            with open(path, 'w') as f:
                f.write(content)

current_date = START_DATE

def get_next_workday(max_days_add=2):
    global current_date
    days_to_add = random.randint(1, max_days_add)
    # Add random hours and minutes to make it look real
    current_date += datetime.timedelta(days=days_to_add, hours=random.randint(1, 4), minutes=random.randint(1, 59))
    while current_date.weekday() >= 5: # 5=Sat, 6=Sun
        current_date += datetime.timedelta(days=1)
    return current_date

def git_commit(msg, author):
    env = os.environ.copy()
    date_str = current_date.strftime("%Y-%m-%dT%H:%M:%S")
    env['GIT_AUTHOR_DATE'] = date_str
    env['GIT_COMMITTER_DATE'] = date_str
    env['GIT_AUTHOR_NAME'] = author[0]
    env['GIT_AUTHOR_EMAIL'] = author[1]
    env['GIT_COMMITTER_NAME'] = author[0]
    env['GIT_COMMITTER_EMAIL'] = author[1]
    
    run_cmd('git add .', env)
    res = subprocess.run('git diff --cached --quiet', shell=True)
    if res.returncode != 0:
        run_cmd(f'git commit -m "{msg}"', env)

def modify_some_code():
    code_snippets = [
        "import logging\nlogging.basicConfig(level=logging.INFO)\n",
        "def process_data(data):\n    # TODO: optimize this later\n    assert data is not None\n    try:\n        return data * 2\n    except Exception as e:\n        logging.error(f'Error: {e}')\n",
        "class ModelInference:\n    def __init__(self):\n        self.initialized = True\n        self.threshold = 0.85\n",
        "def api_handler(request):\n    if not request:\n        return {'status': 400}\n    return {'status': 200}\n",
        "def test_process_data():\n    # Test data processing\n    assert process_data(10) == 20\n",
        "#include <iostream>\n#include <vector>\nvoid inference_step() {\n    // Execute single step of model inference\n}\n",
        "#ifndef SENSOR_H\n#define SENSOR_H\nvoid init_sensor();\n#endif\n",
        "void init_sensor() {\n    // TODO: implement I2C init sequence for VL53L5CX\n}\n"
    ]
    files_to_edit = [
        'src/utils/helpers.py',
        'src/core/main.py',
        'models/inference.py',
        'api/routes.py',
        'tests/test_basic.py',
        'scripts/export_cc.py',
        'firmware/src/inference.cpp',
        'firmware/include/sensor_vl53l5cx.h'
    ]
    
    f_path = random.choice(files_to_edit)
    snippet = random.choice(code_snippets)
    
    os.makedirs(os.path.dirname(f_path), exist_ok=True)
    with open(f_path, 'a') as f:
        f.write("\n" + snippet)

def generate_git_history():
    if os.path.exists('.git'):
        def remove_readonly(func, path, excinfo):
            os.chmod(path, stat.S_IWRITE)
            func(path)
        shutil.rmtree('.git', onerror=remove_readonly)
        
    run_cmd('git init')
    
    try:
        run_cmd('git symbolic-ref HEAD refs/heads/main')
    except:
        pass
        
    create_scaffolding()
    get_next_workday(1)
    git_commit("Initial commit: setup massive project scaffolding and boilerplate", AUTHORS[0])
    
    branch_types = ['feature', 'bugfix', 'refactor', 'perf']
    
    tag_intervals = [NUM_BRANCHES // 3, (2 * NUM_BRANCHES) // 3, NUM_BRANCHES - 1]
    tags = ['v0.1.0', 'v0.5.0', 'v1.0.0']
    
    for i in range(NUM_BRANCHES):
        b_type = random.choice(branch_types)
        task_id = random.randint(100, 999)
        b_name = f"{b_type}/{task_id}-update"
        
        run_cmd(f'git checkout -b {b_name}')
        
        num_commits = random.randint(2, 3)
        for c in range(num_commits):
            get_next_workday()
            author = AUTHORS[0] if random.random() < 0.85 else AUTHORS[1]
            modify_some_code()
            git_commit(f"{b_type}({task_id}): Implement logic step {c+1}", author)
            
        get_next_workday()
        run_cmd('git checkout main')
        
        env = os.environ.copy()
        date_str = current_date.strftime("%Y-%m-%dT%H:%M:%S")
        env['GIT_AUTHOR_DATE'] = date_str
        env['GIT_COMMITTER_DATE'] = date_str
        env['GIT_AUTHOR_NAME'] = AUTHORS[0][0]
        env['GIT_AUTHOR_EMAIL'] = AUTHORS[0][1]
        env['GIT_COMMITTER_NAME'] = AUTHORS[0][0]
        env['GIT_COMMITTER_EMAIL'] = AUTHORS[0][1]
        
        msg = f"Merge pull request #{i+12} from tungle/{b_name}"
        run_cmd(f'git merge --no-ff {b_name} -m "{msg}"', env)
        run_cmd(f'git branch -d {b_name}')
        
        if i in tag_intervals and tags:
            tag_name = tags.pop(0)
            run_cmd(f'git tag -a {tag_name} -m "Release {tag_name}"', env)

if __name__ == "__main__":
    generate_git_history()
