<h2>Project Structure</h2>
<br>├── llm_server.py (FastAPI LLM server script)
<br>├── pre_processing.py (Pre-processing script for C files)
<br>├── prompt_templates.py (LLM Prompts)
<br>├── query_ui.py (Streamlit UI)
<br>├── P3_MCP_Application/cmake-src/src (Input folder for codebase)
<br>├── C_Parsed (folder for pre-processed C files)
<br>├── requirements.txt (Python dependencies)
<br>└── README.md

<h2>Setup</h2>

1. Clone the repository
<pre><code>
git clone repo-url
cd repo-folder
</code></pre>

2. Create and activate a virtual environment (recommended)

<pre><code>
python -m venv venv
source venv/bin/activate   # on Linux/Mac
venv\Scripts\activate      # on Windows
</code></pre>

3. Install dependencies

<pre><code>pip install -r requirements.txt</code></pre>

4. Install GNU C Compiler (GCC)

<pre><code>
sudo apt-get update
sudo apt-get install -y build-essential
sudo apt-get install -y libc6-dev
</code></pre>

<br>If <b>fake_libc_include</b> folder is missing, create it manually.
<br>First, go to pycparser library folder, you can find its location by executing:

<pre><code>
import pycparser
print(pycparser.__file__)
</code></pre>

<br>Then, create the "fake_libc_include" folder:
<pre><code>
mkdir -p fake_libc_include
</code></pre>

<br>Add the fake header files to the folder:
<pre><code>
git clone https://github.com/eliben/pycparser.git
cp -r pycparser/utils/fake_libc_include .
</code></pre>

<h2>Input Data</h2>
Copy the codebase files inside the local path <b>P3_MCP_Application/cmake-src/src</b> into the same repo's path.

<h2>Running the Application</h2>

1. Ensure CUDA is available
2. Start the FastAPI Server
<pre><code>uvicorn llm_server:app --host 0.0.0.0 --port 8000</code></pre>
2. Start the Streamlit UI
<br>Open a new terminal (keep FastAPI running) and run:
<pre><code>streamlit run query_ui.py</code></pre>
<br>The frontend will launch at: http://localhost:8501

<h2>END</h2>
