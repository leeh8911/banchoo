import requests
import time
import subprocess
import signal
import os

BASE_URL = "http://localhost:18080"


def wait_for_server(timeout=5):
    for _ in range(timeout * 10):
        try:
            res = requests.get(BASE_URL + "/notes")
            if res.status_code == 200:
                return True
        except:
            pass
        time.sleep(0.1)
    raise RuntimeError("❌ Server did not start in time.")


def test_crud_flow(name, payload):
    base = f"{BASE_URL}/{name}s"

    print(f"🚨 Testing {name.upper()} flow")

    print("✅ Create")
    res = requests.post(base, json=payload)
    assert res.status_code == 200, res.text
    note = res.json()
    assert note["content"] == payload["content"]
    note_id = note["id"]

    print("✅ Read")
    res = requests.get(f"{BASE_URL}/notes/{note_id}")
    assert res.status_code == 200
    assert res.json()["id"] == note_id

    print("✅ Update")
    updated = {"content": f"{name} updated"}
    res = requests.put(f"{BASE_URL}/notes/{note_id}", json=updated)
    assert res.status_code == 200

    print("✅ Read Again")
    res = requests.get(f"{BASE_URL}/notes/{note_id}")
    assert res.status_code == 200
    assert res.json()["content"] == updated["content"]

    print("✅ Delete")
    res = requests.delete(f"{BASE_URL}/notes/{note_id}")
    assert res.status_code == 200

    print("✅ Confirm Deletion")
    res = requests.get(f"{BASE_URL}/notes/{note_id}")
    assert res.status_code == 404


def run_tests():
    test_crud_flow("memo", {"content": "memo content"})
    test_crud_flow("task", {
        "content": "task content",
        "status": "TODO",
        "due_date": "2025-04-05T12:00:00"
    })
    test_crud_flow("event", {
        "content": "event content",
        "start_date": "2025-04-06T09:00:00",
        "end_date": "2025-04-06T18:00:00"
    })


if __name__ == "__main__":
    print("⏳ Waiting for server to be ready...")
    wait_for_server()
    run_tests()
    print("✅ All tests passed!")
