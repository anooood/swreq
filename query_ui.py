# ui_app.py
import streamlit as st
import requests
import glob
from pathlib import Path
from pre_processing import pre_processing, generate_functions, extract_header_comments
from templates import name_prompt_template, reqs_prompt_template

SERVER_URL = "http://<server-ip>:8000/generate"

# ───── Collect module names from filenames ─────
Git_folder = "P3_MCP_Application/cmake-src/src"
modules = [Path(f).name for f in glob.glob(Git_folder+"/*.c")]

st.title("MCP Application Requirements Generator")

selected_module = st.selectbox("Select a system/module:", modules)
cleaned_module = selected_module.replace('.c','')
    
if st.button("Generate Requirements"):

    ### Pre-process selected module and generate function chunks
    print(selected_module)
    output_file, input_file = pre_processing(selected_module)
    functions = generate_functions(output_file, input_file)
    header_comments = extract_header_comments(input_file)
    
    st.subheader(f"Requirements for **{selected_module}**")

    with st.spinner("Generating requirements..."):

        ### Loop through extracted function chunks
        for i in range(len(functions)):
            print(functions[i]['name'])

            ### Update prompts
            name_prompt = name_prompt_template.format(
                header_comments = header_comments,
                function_name = functions[i]['name']
            )
            reqs_prompt = reqs_prompt_template.format(
            function_name = functions[i]['name'],
            function_code = functions[i]['content']
            )

            try:

                ### Query LLM API
                response = requests.post(
                    "http://127.0.0.1:8000/generate",
                    json={"prompts": [name_prompt,reqs_prompt]},
                    timeout=300  # allow long responses
                )

                ### Printing response in streamlit UI
                if response.status_code == 200:
                    data = response.json()
                    st.markdown(data['requirements'][0])  # smaller than subheader
                    st.write(data['requirements'][1])
                    with st.expander("Code Reference"):
                        st.code(functions[i]['content'], language="c")
                    st.divider()
                else:
                    st.error(f"Error {response.status_code}: {response.text}")
            except Exception as e:
                st.error(f"Request failed: {e}")