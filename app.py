import datetime
from http import client
import re
from fastapi import FastAPI
from typing import Annotated, List, Optional
import uuid
from fastapi import FastAPI, HTTPException, Response
import motor. motor_asyncio
from dotenv import dotenv_values
from pydantic import BaseModel, BeforeValidator, Field, TypeAdapter 
from bson import ObjectId
from pymongo import ReturnDocument 


app = FastAPI()  

config = dotenv_values(".env")


client = motor.motor_asyncio.AsyncIOMotorClient(config["MONGO_URL"])
db = client.IOT_Project_Database

PyObjectId = Annotated[str, BeforeValidator(str)]

class Settings(BaseModel):
    id: Optional[PyObjectId] = Field(alias = "_id", default = None)
    user_temp: Optional[float] = None
    user_light: Optional[str] = None
    light_time_off: Optional[str] = None
    light_duration: Optional[str] = None

class updatedSettings(BaseModel):
    id: Optional[PyObjectId] = Field(alias = "_id", default = None)
    user_temp: Optional[float] = None
    user_light: Optional[str] = None
    light_time_off: Optional[str] = None

class sensorData(BaseModel):
    id: Optional[PyObjectId] = Field(alias = "_id", default = None)
    temperature: Optional[float] = None
    presence: Optional[bool] = None
    datetime: Optional[str] = None  

regex = re.compile(r'((?P<hours>\d+?)h)?((?P<minutes>\d+?)m)?((?P<seconds>\d+?)s)?')

def parse_time(time_str):
    parts = regex.match(time_str)
    if not parts:
        return
    parts = parts.groupdict()
    time_params = {}
    for name, param in parts.items():
        if param:
            time_params[name] = int(param)
    return datetime.timedelta(**time_params)


 
def convert24(time):
    # Parse the time string into a datetime object
    t = datetime.strptime(time, '%I:%M:%S %p')
    # Format the datetime object into a 24-hour time string
    return t.strftime('%H:%M:%S')

@app.post("/sensorData",status_code=201)
async def createSensorData(sensor_data:sensorData):
    entry_time = datetime.now().strftime("%d/%m/%Y, %H:%M:%S")
    sensor_data_ = sensor_data.model_dump()
    sensor_data_["datetime"] = entry_time
    new_data = await db["sensorData"].insert_one(sensor_data_)
    created_data = await db["sensorData"].find_one({"_id": new_data.inserted_id})
    return sensorData(**created_data)

