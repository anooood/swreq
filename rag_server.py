# rag_server.py
from pycparser import c_parser, c_ast
import re
from langchain.vectorstores import Chroma
from langchain.embeddings import HuggingFaceEmbeddings
from langchain.llms import HuggingFacePipeline
from langchain.chains import RetrievalQA
from transformers import pipeline, AutoTokenizer, AutoModelForCausalLM
from langchain.docstore.document import Document
from pathlib import Path
import torch, gc
from langchain.text_splitter import CharacterTextSplitter
import os
import shutil
import glob
import streamlit as st
from fastapi import FastAPI
from pydantic import BaseModel
from typing import List

# ───── Load CodeLlama ─────
model_id = "codellama/CodeLlama-7b-Instruct-hf"  # or 13B if GPU has enough VRAM

tokenizer = AutoTokenizer.from_pretrained(model_id)

model = AutoModelForCausalLM.from_pretrained(
    model_id,
    device_map="auto",
    load_in_8bit=True   # reduces GPU memory usage
)

llm_pipeline = pipeline(
    "text-generation",
    model=model,
    tokenizer=tokenizer,
    max_new_tokens=1024,
    temperature=0.0,
    do_sample=False,
    repetition_penalty=1.2,
    pad_token_id=tokenizer.eos_token_id,
)

llm = HuggingFacePipeline(pipeline=llm_pipeline)

# ───── API Endpoint ─────
app = FastAPI()

class Query(BaseModel):
    prompts: List[str]

@app.post("/generate")
def generate(query: Query):

    function_name = llm(query.prompts[0]).split('### Output')[1]
    header = function_name.replace('"','').replace("1. ","").strip()

    req_response = llm(query.prompts[1])
    response = req_response.split('### Requirements:')[1]

    return {"requirements": [header, response]}