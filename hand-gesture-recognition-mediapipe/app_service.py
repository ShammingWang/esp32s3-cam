from fastapi import FastAPI, Request
from fastapi.responses import JSONResponse
from datetime import datetime
import os
import cv2 as cv
import numpy as np
import mediapipe as mp
from model import KeyPointClassifier  # 假设你已经训练了一个手势分类模型
import io

app = FastAPI()

UPLOAD_DIR = "uploads"
os.makedirs(UPLOAD_DIR, exist_ok=True)

# 初始化变量
mp_hands = None
hands = None
keypoint_classifier = None

async def lifespan(app: FastAPI):
    """Lifespan event handler for startup and shutdown."""
    global mp_hands, hands, keypoint_classifier

    # 在应用启动时加载模型
    mp_hands = mp.solutions.hands
    hands = mp_hands.Hands(static_image_mode=False, max_num_hands=1, min_detection_confidence=0.7, min_tracking_confidence=0.5)
    keypoint_classifier = KeyPointClassifier()
    print("Model loaded successfully.")

    # 在应用关闭时释放模型资源
    yield
    if hands:
        hands.close()
        print("Hands model released.")
    if keypoint_classifier:
        keypoint_classifier.close()
        print("Keypoint classifier model released.")

app = FastAPI(lifespan=lifespan)

def draw_landmarks(image: np.ndarray, landmark_point: list):
    """在图像上绘制手部关键点"""
    for index, landmark in enumerate(landmark_point):
        # 转换为整数坐标
        x, y = int(landmark[0] * image.shape[1]), int(landmark[1] * image.shape[0])
        cv.circle(image, (x, y), 5, (0, 255, 0), -1)  # 绘制每个关键点

    return image

def classify_gesture(image: np.ndarray):
    from app import pre_process_landmark  # 确保在这里导入预处理函数
    """根据手部关键点分类手势"""
    image = cv.cvtColor(image, cv.COLOR_BGR2RGB)

    results = hands.process(image)
    
    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            landmarks = []
            for landmark in hand_landmarks.landmark:
                landmarks.append([landmark.x, landmark.y])

            # 可视化：绘制手部关键点
            debug_image = draw_landmarks(image, landmarks)

            # 进行手势分类
            pre_processed_landmarks = pre_process_landmark(landmarks)
            hand_sign_id = keypoint_classifier(pre_processed_landmarks)
            gesture = "open" if hand_sign_id == 0 else "close"

            return gesture, debug_image
    
    return "no hand detected", image

@app.post("/upload")
async def upload_image(request: Request):
    """接收图像并保存可视化结果"""
    try:
        # 获取上传的文件内容
        content = await request.body()

        # 将内容转换为内存中的图像
        image = np.frombuffer(content, np.uint8)
        image = cv.imdecode(image, cv.IMREAD_COLOR)

        # 进行手势识别并获取可视化结果
        gesture_result, debug_image = classify_gesture(image)
        
        print(gesture_result)

        # 保存可视化结果图像到本地
        filename = f"{datetime.now().strftime('%Y%m%d_%H%M%S')}_debug.jpg"
        filepath = os.path.join(UPLOAD_DIR, filename)
        
        # cv.imwrite(filepath, debug_image)  # 保存可视化图像

        return JSONResponse(content={"message": "Image uploaded", "gesture": gesture_result, "filename": filename})
    
    except Exception as e:
        return JSONResponse(content={"error": str(e)}, status_code=500)

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
