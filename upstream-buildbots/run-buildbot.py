#!/usr/bin/env python3

import os
from docker import from_env
from docker.errors import APIError, ImageNotFound


def buildUbu22HIP():
  ImgTag = 'ubu/22/hip'
  BuildContext = './Ubu22'
  DockerfilePath = os.path.join(BuildContext, 'Dockerfile')

  try:
    Client = from_env()
    print("Building {} from {}".format(ImgTag, DockerfilePath))
    Image, BuildLogs = Client.images.build(path=BuildContext, tag=ImgTag)
    print("Finished building {}".format(ImgTag))

  except APIError as e:
    print("Something went wrong.\n{}".format(e))
