# rag_server.py
from transformers import pipeline, AutoTokenizer, AutoModelForCausalLM
import torch
from fastapi import FastAPI
from pydantic import BaseModel
from typing import List

# ───── Load Model ─────
model_path = "../models/mistralai/Mistral-7B-Instruct-v0.3"

tokenizer = AutoTokenizer.from_pretrained(model_path)

model = AutoModelForCausalLM.from_pretrained(
    model_path,
    trust_remote_code=True,
    torch_dtype=torch.float16,
    device_map="auto"
)

pipe = pipeline(
    "text-generation",
    model=model,
    tokenizer=tokenizer,
)
        
GEN_CONFIG = dict(
    do_sample=False,
    temperature=0.0,
    top_p=1.0,
    repetition_penalty=1.1,
    max_new_tokens=4096,
    pad_token_id=tokenizer.eos_token_id
)

# ───── API Endpoint ─────
app = FastAPI()

class Query(BaseModel):
    prompts: List[str]
    
@app.post("/generate")
def generate(query: Query):
        
    with torch.no_grad():
        
        if query.prompts[0] != 'N/A':
            header = pipe(
                query.prompts[0],
                **GEN_CONFIG,
                return_full_text=False
            )[0]["generated_text"]
            
            if ":" in header:
                header = header.split(':')[-1]
        
        else:
            header = 'N/A'

        requirements = pipe(
            query.prompts[1],
            **GEN_CONFIG,
            return_full_text=False
        )[0]["generated_text"]

        return {"requirements": [header, requirements]}