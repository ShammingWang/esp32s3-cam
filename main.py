from fastapi import FastAPI, Request
from fastapi.responses import JSONResponse
from datetime import datetime
import os

app = FastAPI()

UPLOAD_DIR = "uploads"
os.makedirs(UPLOAD_DIR, exist_ok=True)

@app.post("/upload")
async def upload_image(request: Request):
    try:
        content = await request.body()
        filename = f"{datetime.now().strftime('%Y%m%d_%H%M%S')}.jpg"
        filepath = os.path.join(UPLOAD_DIR, filename)

        with open(filepath, "wb") as f:
            f.write(content)

        return JSONResponse(content={"message": "Image uploaded", "filename": filename})
    except Exception as e:
        return JSONResponse(content={"error": str(e)}, status_code=500)

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
