#!/usr/bin/env python3

import os
import time
import argparse
from docker import from_env
from docker.errors import APIError, ImageNotFound


class BBConfig:
    def __init__(
        self,
        ImgTag,
        LLVMCMakeCfg=None,
        LLVMTestSuiteCMakeCfg=None,
        TestCmd=None,
        Dockerfile=None,
    ):
        self.ImgTag = ImgTag
        self.LLVMCMakeCfg = LLVMCMakeCfg
        self.LLVMTestSuiteCMakeCfg = LLVMTestSuiteCMakeCfg
        self.TestCmd = TestCmd
        self.Dockerfile = Dockerfile

    def getImgTag(self):
        return self.ImgTag

    def getLLVMCMakeCfg(self):
        return self.LLVMCMakeCfg

    def getLLVMTestSuiteCMakeCfg(self):
        return self.LLVMTestSuiteCMakeCfg

    def getTestCmd(self):
        return self.TestCmd

    def getDockerfile(self):
        return self.Dockerfile


Buildbots = {
    "hip-simple": BBConfig("ubu/22/buildbot/hip"),
    "offload": None,
    "libc": None,
}


def getBuildbotsAsList():
    return [k for k in Buildbots.keys()]


def buildUbu22HIP():
    ImgTag = Buildbots["hip-simple"].getImgTag()
    BuildContext = "./Ubu22"
    DockerfilePath = os.path.join(BuildContext, "Dockerfile")

    try:
        Client = from_env()
        print("Building {} from {}".format(ImgTag, DockerfilePath))

        Image, BuildLogs = Client.images.build(path=BuildContext, tag=ImgTag)
        print("Finished building {} with container ID {}".format(ImgTag, Image.id))

    except APIError as E:
        print("Something went wrong.\n{}".format(e))
        exit(1)

    return Client


def cloneAndBuildLLVM(DockerClient):
    try:
        Container = DockerClient.containers.run(
            image=Buildbots["hip-simple"].getImgTag(),
            detach=True,
            name="bbot-hip-simple",
            ports={},
            tty=True,
            stdin_open=True,
            remove=False,
            cpuset_cpus='0-31',
        )

        time.sleep(5)
        print("Container status: {}".format(Container.status))

        print("Cloning LLVM")
        Container.exec_run("git clone --single-branch --depth 1 https://github.com/llvm/llvm-project.git", user='botworker', workdir='/home/botworker')
        print(" ... complete")

        print("Configuring LLVM")
        Container.exec_run("mkdir build", user='botworker', workdir='/home/botworker/llvm-project')
        Container.exec_run("cmake -GNinja -C ../offload/cmake/caches/AMDGPUBot.cmake ../llvm", user='botworker', workdir='/home/botworker/llvm-project/build')
        print(" ... done.")

        print("Building LLVM")
        EC, Output = Container.exec_run("ninja", user='botworker', workdir='/home/botworker/llvm-project/build')
        if EC != 0:
            print(Output)
        print(" ... done.")

    except APIError as E:
        print("Error in running the container.\n{}".format(E))
        exit(1)


def main():
    # Create argument parser for this tool
    Parser = argparse.ArgumentParser(
        description="Reproduce buildbot builds in a local container"
    )
    # Select which buildbot to create
    Parser.add_argument(
        "buildbot",
        help="Which buildbot. Determines Dockerfile. Options: "
        + str(getBuildbotsAsList()),
    )
    Args = Parser.parse_args()

    if not Args.buildbot in Buildbots.keys():
        print("Error: Unknown buildbot. Use spelling from " + str(getBuildbotsAsList()))
        exit(1)

    if Args.buildbot == "hip-simple":
        DockerClient = buildUbu22HIP()
        cloneAndBuildLLVM(DockerClient)


if __name__ == "__main__":
    main()
