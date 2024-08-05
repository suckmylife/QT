from typing import List

class DataStack:
    _shared_instance = None

    def __new__(cls):
        if cls._shared_instance is None:
            cls._shared_instance = super().__new__(cls)
            cls._shared_instance._stack: List[bytes] = []
        return cls._shared_instance

    def push(self, data: bytes):
        """데이터를 스택에 추가합니다."""
        self._stack.append(data)

    def pop(self):
        """스택에서 데이터를 꺼냅니다. 가장 최근에 추가된 데이터가 반환됩니다."""
        if self._stack:
            return self._stack.pop()
        else:
            return None

    def clear(self):
        """스택의 모든 데이터를 지웁니다."""
        self._stack = []

    def get_stack(self):
        """스택의 현재 상태를 반환합니다."""
        return self._stack
