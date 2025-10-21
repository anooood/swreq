<h2>Project Structure</h2>
<br>├── rag_server.py (FastAPI LLM server script)
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

<h2>Input Data</h2>
Copy the codebase files inside the local path <b>P3_MCP_Application/cmake-src/src</b> into the same repo's path.

<h2>Running the Application</h2>

1. Ensure CUDA is available
2. Start the FastAPI Server
<pre><code>uvicorn rag_server:app --host 0.0.0.0 --port 8000</code></pre>
2. Start the Streamlit UI
<br>Open a new terminal (keep FastAPI running) and run:
<pre><code>streamlit run query_ui.py</code></pre>
<br>The frontend will launch at: http://localhost:8501

<h2>END</h2>
