from multiprocessing import shared_memory


# wrapper class for shared memory Python. Added a convenient write method
class shmem:
    def __init__(self, name="shmem", create=False, size=0):
        self.shm = shared_memory.SharedMemory(name=name, create=create, size=size)

    def name(self):
        return self.shm.name

    def size(self):
        return self.shm.size

    def buf(self):
        return self.shm.buf

    def write(self, msg):
        buf = bytearray(self.shm.buf.nbytes)
        buf[:len(msg)] = bytearray(msg, 'utf-8')
        self.shm.buf[:] = buf

    def read(self):
        return bytes(self.shm.buf).strip(b'\x00').decode('ascii')

    def close(self):
        self.shm.close()

    def unlink(self):
        self.shm.unlink()


def shmpy():
    print("Hello World from Python!")

    shm_force = shmem(name="force")

    while True:
        if shm_force.read() == "shm created":
            break

    shm_force.write("accessed")

    previous = "accessed"

    while True:
        current = shm_force.read()

        if current == "terminate":
            break

        if current != previous:
            print(f"PYTHON: RECEIVED '{current}' FROM SHARED MEMORY")

        previous = current

    shm_force.write("affirmative")
    shm_force.close()
    shm_force.unlink()


if __name__ == "__main__":
    shmpy()
