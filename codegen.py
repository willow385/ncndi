import os
from ast_utils import *
from ast_parser import *

class CodeGenerator:
  def __init__(self, abstract_syntax_tree: Program, var_scope, funct_scope):
    self.ast = abstract_syntax_tree
    self.variable_scope = var_scope
    self.function_scope = funct_scope

  def _compile_node(self, node):
    result = str()
    if type(node) is Function:
      result += f"""\n; function {node.function_name}()
@{node.function_name}:
"""
      result += self._compile_node(node.body)
      result += "  pop ip\n\n"
    else:
      if type(node) is str:
        result += f'\n; unimplemented feature: {node}\n'
      else:
        result += f"\n; unimplemented feature: {type(node)}\n"
    return result

  def compile(self):
    assembly = AssemblyCode()
    assembly.code += """; tma-16 assembly code
; Code generated by the MPL compiler

; entry point of the program
jmp @__start__
    """

    main_section = Program()
    for node in self.ast.children:
      if type(node) is Function:
        self.function_scope[node.function_name] = node
      else:
        main_section.children.append(node)

    for function_id in self.function_scope.keys():
      assembly.code += self._compile_node(self.function_scope[function_id])

    assembly.code += """; entry point of the program
@__start__:
"""
    assembly.code += self._compile_node(main_section)
    assembly.code += "  halt ; end of program\n"

    return assembly


class AssemblyCode:
  def __init__(self):
    self.code = str()

  # `of` is the path to which we write our code
  def assemble(self, of: str, preserve_asm=False, gen_tmx=True, assembler_opts=""):
    asm_file_path = of.replace(".tmx", ".asm")
    print("Writing assembly file...")
    asm_file = open(asm_file_path, "w+")
    asm_file.write(self.code)
    asm_file.close()
    if gen_tmx:
      print("Assembling...")
      os.system(f"tmasm {asm_file_path} {assembler_opts}")
      if not preserve_asm:
        print("Removing assembly file...")
        os.system(f"rm {asm_file_path}")
      print("Finished assembling.")
    else:
      print("Assembly generated.")