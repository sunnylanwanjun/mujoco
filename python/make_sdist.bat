@echo off
setlocal enabledelayedexpansion
setlocal enableextensions

:: Copyright 2022 DeepMind Technologies Limited
::
:: Licensed under the Apache License, Version 2.0 (the "License");
:: you may not use this file except in compliance with the License.
:: You may obtain a copy of the License at
::
::     https://www.apache.org/licenses/LICENSE-2.0
::
:: Unless required by applicable law or agreed to in writing, software
:: distributed under the License is distributed on an "AS IS" BASIS,
:: WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
:: See the License for the specific language governing permissions and
:: limitations under the License.

:: 检查是否在虚拟环境中运行
if "%VIRTUAL_ENV%"=="" (
    if "%CONDA_DEFAULT_ENV%"=="" (
        echo This script must be run from within a Python virtual environment
        exit /b 1
    )
)

:: 获取脚本所在目录
set "package_dir=%~dp0"
set "package_dir=%package_dir:~0,-1%"

:: 创建临时目录
set "tmp_dir=%LOCALAPPDATA%\Temp\mujoco_temp_%RANDOM%%RANDOM%"
mkdir "%tmp_dir%"

:: 更新pip并安装依赖
python -m pip install --upgrade --require-hashes -r "%package_dir%\make_sdist_requirements.txt"

:: 进入临时目录
pushd "%tmp_dir%"

:: 复制包目录内容
xcopy /s /e /i /y "%package_dir%\*" .

:: 保存并设置PYTHONPATH以生成头文件
set "old_pythonpath=%PYTHONPATH%"
set "PYTHONPATH=%old_pythonpath%;%package_dir%\mujoco\python\.."

:: 生成头文件
python "%package_dir%\mujoco\codegen\generate_enum_traits.py" > mujoco\enum_traits.h
python "%package_dir%\mujoco\codegen\generate_function_traits.py" > mujoco\function_traits.h
python "%package_dir%\mujoco\codegen\generate_spec_bindings.py" > mujoco\specs.cc.inc

:: 恢复PYTHONPATH
set "PYTHONPATH=%old_pythonpath%"

:: 复制LICENSE文件
copy "%package_dir%\..\LICENSE" .

:: 复制CMake脚本
mkdir mujoco\cmake
copy "%package_dir%\..\cmake\*.cmake" mujoco\cmake

:: 复制Simulate源代码
xcopy /s /e /i /y "%package_dir%\..\simulate" mujoco\simulate

:: 构建源码分发包
python -m build . --sdist

:: 列出分发包内容
tar -tf dist\mujoco-*.tar.gz

:: 返回原目录
popd

:: 创建dist目录并移动生成的分发包
mkdir "%package_dir%\dist" 2>nul
move "%tmp_dir%\dist\*" "%package_dir%\dist"

:: 清理临时目录
rmdir /s /q "%tmp_dir%"