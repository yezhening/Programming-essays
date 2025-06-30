# 1.准备知识库————————————————————
def read_knowledge_base() -> str:
    with open("knowledge_base.md", "r", encoding="utf-8") as f:
        return f.read()


# 2.chunking分块/段————————————————————
def divide_chunks() -> list[str]:
    text = read_knowledge_base()
    tmp_chunks = text.split("\n\n")

    # title不分块，和下一块合并
    chunks = []
    title = ""
    for c in tmp_chunks:
        if c.startswith("#"):
            title += f"{c}\n"  # 记录
        else:
            chunks.append(f"{title}{c}")  # 合并
            title = ""  # 重置

    return chunks


# if __name__ == "__main__":
#     chunks = divide_chunks()
#     for c in chunks:
#         print(c)
#         print("---------------")


# 3.embedding嵌入————————————————————
from google import genai

# 获取api密钥：https://aistudio.google.com/apikey（需要魔法）
# 作者已删除该密钥
genai_client = genai.Client(api_key="AIzaSyDOai6QVu_r_fMRbFeLv5-rSyljpqpAhWc")
EMBED_MODEL = "gemini-embedding-exp-03-07"


def embed(text: str, store: bool) -> list[float]:  # 模型分为存储和查询模式
    result = genai_client.models.embed_content(
        model=EMBED_MODEL,
        contents=text,
        config={"task_type": "RETRIEVAL_DOCUMENT" if store else "RETRIEVAL_QUERY"},
    )

    assert result.embeddings
    assert result.embeddings[0].values
    return result.embeddings[0].values


# if __name__ == "__main__":
#     chunks = divide_chunks()
#     print(embed(chunks[0], store=True))


import chromadb

chromadb_client = chromadb.PersistentClient("./chroma.db")  # 库
chromadb_collection = chromadb_client.get_or_create_collection("linghuchong")  # 表


def store_db() -> None:
    chunks = divide_chunks()

    for idx, c in enumerate(chunks):
        print(f"Process: {c}")
        vec = embed(c, store=True)
        chromadb_collection.upsert(ids=str(idx), documents=c, embeddings=vec)


# if __name__ == "__main__":
#     store_db()  # 可能会因为网络失败


# 4.检索测试
# 5.查询同理，匹配相似度高/向量距离近/数据库相近的几条


def query_db(question: str) -> list[str]:
    vec = embed(question, store=False)
    result = chromadb_collection.query(query_embeddings=vec, n_results=5)

    assert result["documents"]
    return result["documents"][0]


LLM_MODEL = "gemini-2.5-flash-preview-05-20"
if __name__ == "__main__":
    # store_db()

    question = "令狐冲领悟了什么魔法？"
    context = query_db(question)

    system_prompt = "You are clever\n"
    user_prompt = "Please answer user's question according to context\n"
    user_prompt += f"Question: {question}\n"
    user_prompt += "Context:\n"
    for c in context:
        user_prompt += f"{c}\n"
        user_prompt += "-------------\n"
    prompt = system_prompt + user_prompt

    result = genai_client.models.generate_content(model=LLM_MODEL, contents=prompt)
    print(result)
    # 没有继续提取answer了
