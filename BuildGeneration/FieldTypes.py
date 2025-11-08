from enum import Enum


class ETypePrimitive(Enum):
    PRIMITIVE = 0
    ARRAY = 1

class VariableField:
    def __init__(self):
        self.NameVar = ""
        self.Type = ""
        self.Params = ""
        self.TypePrimitive = ETypePrimitive.PRIMITIVE

class ArrayType(VariableField):
    def __init__(self):
        super().__init__()
        self.InnerType = ""

class ClassField:
    def __init__(self):
        self.Name = ""
        self.Namespace = ""
        self.IsPointer = False
        self.LineGenBody = MacrosData()
        self.Parent = ""
        self.Variable:VariableField = []
    def __str__(self):
        return f"Name: {self.Name}    Namespace: {self.Namespace}"

    def IsValidGeneretedBody(self):
        return self.LineGenBody.Location != -1

class MacrosData:
    def __init__(self):
        self.Location = 0
        self.Name = ""