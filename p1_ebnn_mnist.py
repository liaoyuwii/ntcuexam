"""Problem 1: EBNN MNIST Single Board Feeder

Publishes MNIST images to single board for inference.
"""

import numpy as np
from custom_msgs.msg import MnistImage
from numpy.typing import NDArray
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, HistoryPolicy, QoSProfile, ReliabilityPolicy

from srecruit_host.config import load_config


class P1FeederNode(Node):
    """Feeder node for Problem 1: Single board MNIST."""
    # TODO: Implement the publisher and timer to send images to client_a at the specified rate

    def __init__(self, images: list[NDArray[np.uint8]], images_per_second: float):
        """Initialize feeder for Problem 1.

        Args:
            images: List of MNIST images (28x28 numpy arrays)
            images_per_second: Rate at which to publish images (Hz)
        """
        super().__init__("p1_feeder")


def create_node(images: list[NDArray[np.uint8]]) -> Node:
    """Factory function for Problem 1 feeder node.

    Args:
        images: List of MNIST images

    Returns:
        Configured P1FeederNode instance
    """
    images_per_second = load_config().publisher["p1"]

    return P1FeederNode(images, images_per_second)
