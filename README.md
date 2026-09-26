<!-- Batch Selector - A GUI batch file manager and launcher for Windows. -->
<!-- Batch Selector is a Windows utility for managing and executing batch files with drag & drop, multiple file processing, parallel execution, batch pads, and remote job execution. -->

# <img width="288" height="192" alt="batch_selector_s" src="https://github.com/user-attachments/assets/0602d807-4149-4636-bd4b-39d10731f1ae" />

![License](https://img.shields.io/badge/license-GPL--2.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange.svg)

**Windows向けのGUIバッチファイル管理・実行ツール**

ドラッグ＆ドロップでバッチファイルを簡単に実行できます。  
複数のバッチファイルをまとめて管理・実行でき、並列処理やバッチパッド、リモートJOB実行などにも対応しています。

---

## 概要

**Batch Selector（バッチ実行セレクター）** は、Windows上でバッチファイル（`.bat`）をGUIから簡単に実行するためのツールです。

通常、バッチファイルを実行するには、エクスプローラーからファイルを探したり、コマンドプロンプトからコマンドを入力したりする必要があります。

Batch Selectorでは、バッチファイルをウィンドウへ**ドラッグ＆ドロップ**するだけで実行できます。

また、複数のバッチファイルを扱うための機能や、バッチ処理を効率よく行うための機能を備えています。

---



## 主な特徴

### ドラッグ＆ドロップで実行

バッチファイルをBatch Selectorのウィンドウへドラッグ＆ドロップするだけで実行できます。

面倒なコマンド入力を行う必要がありません。

### 複数のバッチファイルに対応

複数のバッチファイルをまとめて扱うことができます。

大量のバッチ処理を行う場合にも便利です。

### 並列処理

複数のバッチファイルを並列に実行することができます。

複数の処理を効率よく実行したい場合に利用できます。

### バッチ実行パッド

よく使用するバッチファイルを登録しておき、ボタン操作で簡単に実行できる「バッチ実行パッド」を利用できます。

日常的に使用するバッチ処理をまとめておくと便利です。

### リモートJOB実行

リモート環境に対してJOBを実行するための機能を備えています。

複数のPCや処理環境を利用したバッチ処理にも対応できます。

### バッチファイルへのパラメータ渡し

ドラッグ＆ドロップしたファイルやフォルダーなどの情報を、バッチファイルへパラメータとして渡すことができます。

Batch Selector独自のパラメータ指定にも対応しています。

---

## このような用途に

Batch Selectorは、例えば次のような用途で利用できます。

- ファイルをドラッグ＆ドロップしてバッチ処理を実行したい
- 複数のバッチファイルをまとめて実行したい
- 複数の処理を並列に実行したい
- よく使うバッチファイルをボタンから実行したい
- バッチ処理をGUIから簡単に操作したい
- バッチファイルへファイル名やフォルダー名を渡したい
- 複数の処理環境でJOBを実行したい

---

## 動作環境

- Microsoft Windows

Windows上で動作するスタンドアロンアプリケーションとして利用できます。

---

## ダウンロード

- [bat_sel121b_20260915.zip](https://github.com/sky-seeker99/batch_selector/releases/download/ver1.21b_20260915/bat_sel121b_20260915.zip)

---

## 使い方

### 基本的な使い方

1. Batch Selectorを起動します。
2. 実行したいバッチファイルを用意します。
3. バッチファイルをBatch Selectorのウィンドウへドラッグ＆ドロップします。
4. バッチファイルが実行されます。

複数のファイルをドラッグ＆ドロップして処理することもできます。

---

## バッチ実行パッド

**バッチ実行パッド（bat_pad）** を使用すると、よく使用するバッチファイルを登録しておき、GUIから簡単に実行できます。

日常的に使用するバッチ処理を登録しておけば、毎回エクスプローラーからバッチファイルを探す必要がありません。

---

## バッチファイルのパラメータ

Batch Selectorでは、ドラッグ＆ドロップされたファイルやフォルダーなどの情報をバッチファイルへ渡すことができます。

例えば、通常のバッチファイルで使用できる `%1` などのパラメータに加えて、Batch Selector独自のパラメータを利用できます。

詳しいパラメータや特殊な制御方法については、付属のマニュアルを参照してください。

---

## リモートJOB実行

Batch Selectorには、リモート環境でJOBを実行するための機能があります。

`bat_sel_remote.exe` を使用することで、Batch Selectorからリモート環境へのJOB実行を行うことができます。

詳細については、リモートJOB実行に関するマニュアルを参照してください。

---

## マニュアル

詳しい操作方法や各機能については、以下のマニュアルを参照してください。

- **バッチ実行セレクター マニュアル**
  - [bat_sel_manual.md](https://github.com/sky-seeker99/batch_selector/blob/main/bat_sel_manual.md)

- **バッチ実行パッド マニュアル**
  - [bat_pad_manual.md](https://github.com/sky-seeker99/batch_selector/blob/main/bat_pad_manual.md)
  
---

## 10. ファイル構成

主なファイル・フォルダーは以下のとおりです。

```text
batch_selector/
├─ bat_sel/
│  └─ バッチ実行セレクター
├─ bat_pad/
│  └─ バッチ実行パッド
├─ csvpp_xbase/
├─ bat_sel_manual.md
├─ bat_pad_manual.md
├─ bat_sel.pj2
├─ README.md
└─ LICENSE
```

---

## 特徴的な機能

Batch Selectorでは、通常のバッチファイル実行だけでなく、バッチ処理をより柔軟に行うための機能を備えています。

### ファイル情報の取得

ドラッグ＆ドロップされたファイルについて、

- ファイル名
- フォルダー
- 拡張子
- ファイルパス

などの情報を利用できます。

### 特殊コマンド

バッチファイル内に専用の記述を行うことで、Batch Selectorの機能を利用できます。

例：

```text
!#file_begin
!#file_end
!#proc
!#parainh
```

これらを利用することで、通常のバッチファイルだけでは難しい処理にも対応できます。

---

## ライセンス

本ソフトウェアのライセンスについては、リポジトリ内の `LICENSE` ファイルを参照してください。

---

## 作者

**sky-seeker99**

GitHub:

https://github.com/sky-seeker99/batch_selector

---

## 履歴

今後、機能追加や改善を行っていく予定です。

---

## 最後に

Batch Selectorは、Windowsでバッチファイルをよく利用する方が、**「もっと簡単にバッチを実行したい」**という目的で利用できるツールです。

ドラッグ＆ドロップによる簡単な操作から、複数バッチの実行、並列処理、バッチパッド、リモートJOB実行まで、さまざまなバッチ処理をサポートします。

ぜひご利用ください。

---

**Batch Selector**  
*Windows batch files, made easier.*

