#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Module Description.

This module demonstrates a basic structure following the Google Python Style Guide.
It includes examples of function definitions, docstrings, and main function usage.

Usage:

    $ python template.py
"""

import sys

def example_function(param1, param2):
    """Demonstrates a simple function following Google's Python style.

    Args:
        param1 (int): The first parameter.
        param2 (str): The second parameter.

    Returns:
        bool: The return value. True for success, False otherwise.

    Raises:
        ValueError: If `param1` is less than 0.
    """
    if param1 < 0:
        raise ValueError('param1 must be non-negative')
    return param2.isnumeric()

def main():
    """Main function demonstrating command line argument parsing and usage."""
    if len(sys.argv) != 3:
        print("Usage: python template.py <param1> <param2>")
        sys.exit(1)

    param1 = int(sys.argv[1])
    param2 = sys.argv[2]

    result = example_function(param1, param2)
    print(f"Result of example_function: {result}")

if __name__ == "__main__":
    main()
