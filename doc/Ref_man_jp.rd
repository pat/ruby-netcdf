=begin
=RubyNetCDF レファレンスマニュアル

* ((<メソッドインデックス>))

---------------------------------------------

==概要

RubyNetCDF は NetCDF ライブラリーの Ruby インターフェースである。Ruby 
はフリーなオブジェクト指向スクリプト言語であり、
((<ここ|URL:http://www.ruby-lang.org/>))
から入手できる。RubyNetCDF は数値配列として、
Ruby で一般的に使われている多次元数値配列ライブラリー 
((<NArray|URL:http://www.ruby-lang.org/en/raa-list.rhtml?name=NArray>)) 
を用るので、あらかじめインストールしておく必要がある。
NArray はデータを、C のポインターが指す連続したメモリー領域
に保持し、計算機資源を効率よく利用する。NArray は Python における 
NumPy と似るが、幾つかのテストは NArray のほうが NumPy より効率が良い
ことを示唆している。
オプションとして、RubyNetCDF はデータ欠損の自動的な取り扱いメソッドを
提供する。これを使うには、
((<NArrayMiss|URL:http://ruby.gfd-dennou.org/products/narray_miss/>)) 
が必要である。詳しくは((<使用法>))を見よ。

===構成

RubyNetCDF は以下の４つのクラスから構成される。

* ((<クラス NetCDF>)) -- ファイルのクラス

  一つのNetCDFクラスのオブジェクトは一つの NetCDF ファイルに対応する

* ((<クラス NetCDFDim>)) -- 次元のクラス

  C 版 NetCDF では、次元は、ファイルのIDと次元IDという２変数の組で表されるが、
  Ruby 版では一つの NetCDFDim オブジェクトで代表される

* ((<クラス NetCDFVar>)) -- 変数のクラス

  C 版 NetCDF では、変数は、ファイルのIDと変数IDという２変数の組で表されるが、
  Ruby 版では一つの NetCDFVar オブジェクトで代表される

* ((<クラス NetCDFAtt>)) -- 属性のクラス

  C 版 NetCDF では、属性は、ファイルのIDと変数IDと属性名の組で表されるが、
  Ruby 版では一つの NetCDFAtt オブジェクトで代表される

===データ型

本ライブラリーでは全ての NetCDF 変数の型 char, byte, short, int,
float, double がサポートされている。しかし、これらは Ruby の（より正確
に言うと NArray の）慣例に従って、それぞれ "char", "byte", "sint",
"int", "sfloat", "float" と呼ばれる。従って、NetCDFVar クラスの vartype
(=ntype) はこれらの文字列の一つを返す。また、NetCDFクラスの def_var メ
ソッドは、変数を定義する際、これらを受け付ける。特に注意する必要がある
のは、本ライブラリーでの "float" は C で言うところの double を意味する
ということである。これは Ruby の慣例のせいである -- 組み込みの Float 
クラスは、C の float でなく double に対応するのである。

NetCDFVar クラスの get メソッドはファイル中と同じ型の NArray 変数に値
を読み込むが、"char" 型変数については "byte" 型 NArray に読み込まれる。
これは NArray が "char" 型を持たないからである。ただ、そもそも NArray 
は byte 型で文字列を簡単に扱えるので、特に不都合はないと考えられる。

===エラー処理

エラーは基本的には例外を発生することにより対処する。但し、値を返すメソッ
ドでは軽微なエラーは nil を返すことで対処する（属性値を得るために指定
された名前の属性が存在しないときなど）。例外的な状況で nil を発生する
メソッドについてはマニュアルに明記してある。

===セキュリティ

組み込みの File クラスと同じセキュリティー対応をしていてる。

===使用法

RubyNetCDFライブラリーを利用するためには、まず次の行を Ruby プログラムに書い
てライブラリーをロードする必要がある。

   require 'numru/netcdf'

もしも (NetCDFVarクラスの) 自動的なデータ欠損処理メソッドを使いたい場
合は、以下を実行する必要がある。

   require 'numru/netcdf_miss'

これは内部で一番最初に (({require 'numru/netcdf'})) を実行するので、
これだけ呼べば良い。データ欠損の扱いは
((<NArrayMiss|URL:http://ruby.gfd-dennou.org/products/narray_miss/>))
によるので、これをインストールしておかねばならない。
もしも (({require 'numru/netcdf'})) だけを呼ぶのなら、NArrayMiss
は不要である。

ここで、'numru' ("Numerical Ruby"から取られた) はユーザーのライブラリー
ロードパス中のサブディレクトリーで、RubyNetCDF ライブラリーが置かれる。
すると、以下のように利用できるようになる。

   file = NumRu::NetCDF.create('tmp.nc')
   x = file.def_dim('x',10)
   y = file.def_dim('y',10)
   v = file.def_var('v','float',[x,y])
   file.close

ここで、NumRu はこのライブラリーを含むモジュールである。このようなモジュー
ルにくるんであるのは、名前空間での衝突を避けるためである。このような扱
いをしない場合、もしもユーザーが、本ライブラリー中のクラスと名前が衝突
するクラスやモジュールを含むライブラリーを使おうとすると、問題が起る。

このような問題が起こらないであろう場合は、"NumRu::" という冠は、NumRu
モジュールを「インクルード」することで外せる（現在のスコープにおいてと
いうこである）。従って次のように書ける。

   include NumRu
   file = NetCDF.create('tmp.nc')
   ...

さらなる使用例としてはダウンロード用パッケージに含まれる demo や test 
プログラムを参照せよ。


---------------------------------------------

==マニュアルの見方

--- メソッド名(引数1, 引数2, ...)  -- 省略可能な引数は 引数名=デフォルト値 の形で示す

     機能の解説

     引数
     * 引数1の名 (そのクラスまたは取り得る値): 説明
     * 引数2の名 (そのクラスまたは取り得る値): 説明
     * ...

     戻り値
     * 戻り値の説明

     対応する(利用する) C 版 NetCDF の関数
     * NetCDF ver 3 の関数名。括弧がない場合、その関数と同等の機能を有
       することを示す。直接的な対応がない場合、括弧内に依存する関数を挙げる。

---------------------------------------------

==メソッドインデックス
* ((<クラス NetCDF>))

  クラスメソッド
    * ((<NetCDF.open>))     ファイルオープン（クラスメソッド）mode="w" でファイルが存在しなければ新規作成
    * ((<NetCDF.new>))     NetCDF.openメソッドのエイリアスである
    * ((<NetCDF.create>))     NetCDFファイルを作る（クラスメソッド)
    * ((<NetCDF.create_tmp>))     テンポラリーNetCDFファイルを作る（クラスメソッド)

  インスタンスメソッド
    * ((<close>))     ファイルクローズ
    * ((<ndims>))     ファイル中の次元の数を返す
    * ((<nvars>))     ファイル中の変数の数を返す
    * ((<natts>))     ファイル中のグローバル属性の数を返す
    * ((<unlimited>))     ファイル中のunlimited dimensionを返す
    * ((<path>))     ファイルのパス. open/create時のfilename引数の中身を返す.
    * ((<redef>))     define modeにする。既にそうなら何もせずnilを返す。
    * ((<enddef>))     data mode に入る。既にそうなら何もせずnilを返す。
    * ((<define_mode?>))     今定義モードかどうか問合わせる.
    * ((<sync>))     メモリー中のバッファーをディスク上に反映してファイルを同期させる
    * ((<def_dim>))     dimensionを定義
    * ((<put_att>))     グローバル属性を設定
    * ((<def_var>))     変数を定義
    * ((<def_var_with_dim>))     def_varと同じだが必要ならまず次元を定義する。
    * ((<var>))     ファイルに既存の変数をオープン
    * ((<vars>))    ファイルに既存の変数をまとめてオープン
    * ((<dim>))     既存の次元をオープン
    * ((<dims>))    既存の次元をまとめてオープン
    * ((<att>))     既存のグローバル属性をオープン
    * ((<fill=>))     fillmodeの変更。(NetCDF のデフォルトは FILL である。)
    * ((<each_dim>))     次元に関するイテレータ. 
    * ((<each_var>))     変数に関するイテレータ. 
    * ((<each_att>))     グローバル属性に関するイテレータ. 
    * ((<dim_names>))     ファイル中の全次元の名前を配列に入れて返す。
    * ((<var_names>))     ファイル中の全変数の名前を配列に入れて返す。
    * ((<att_names>))     ファイル中の全グローバル属性の名前を配列に入れて返す。

* ((<クラス NetCDFDim>))

  クラスメソッド

  インスタンスメソッド
    * ((<length>))     次元の長さを返す
    * ((<length_ul0>))     length と同じだが、無制限次元に関しゼロを返す
    * ((<name=>))     名前をつけかえる
    * ((<name>))     名前を返す
    * ((<unlimited?>))     無制限次元かどうか？

* ((<クラス NetCDFVar>))

  クラスメソッド
    * ((<NetCDFVar.new>))     NetCDF.open と NetCDF#Var を組み合わせて一行で済ます(使わなくて良い).
    * ((<NetCDFVar.unpack_type=>))    ((<unpack>))で使うNArrayの型を固定する
    * ((<NetCDFVar.unpack_type>))    ((<NetCDFVar.unpack_type=>))で設定したNArrayの型を返す


  インスタンスメソッド
    * ((<dim>))     その変数における dim_num 番目(0から数える)の次元を問合わせる。
    * ((<dims>))     その変数の全次元を配列にいれて返す
    * ((<shape_ul0>))     変数の形を返す. 但し無制限次元の長さはゼロ.
    * ((<shape_current>))     変数の現在の形を返す.
    * ((<each_att>))     変数中の全属性(NetCDFAtt)に関するイテレータ
    * ((<dim_names>))     変数中の全次元の名前を配列に入れて返す。
    * ((<att_names>))     変数中の全属性の名前を配列に入れて返す。
    * ((<name>))     変数の名前を返す
    * ((<name=>))     名前を付け替える
    * ((<ndims>))     次元の数を問う
    * ((<rank>))     ndimsのエリアス
    * ((<ntype>))     vartype の別名
    * ((<vartype>))     変数値の型を問う
    * ((<typecode>))     変数値の型を問う(NArrayのtypecodeで返す)
    * ((<natts>))     属性の数を問う
    * ((<file>))     その変数が属するファイルを問合わせる
    * ((<att>))     名前を指定した属性を返す
    * ((<put_att>))     属性を設定
    * ((<put>))     　((<simple_put>))の別名(alias)
    * ((<simple_put>))     値を入れる
    * ((<pack>)) selfの属性 scale_factor and/or add_offset を用いて NArray 等を "pack" する.
    * ((<scaled_put>))     ((<simple_put>)) と同様だが、((<pack>))により属性 scale_factor と add_offset を解釈する
    * ((<get>))     　((<simple_get>))の別名(alias)
    * ((<simple_get>))     値を取り出す
    * ((<unpack>)) selfの属性 scale_factor and/or add_offset を用いて NArray 等を "unpack" する.
    * ((<scaled_get>))     ((<simple_get>)) と同様だが、((<unpack>))により属性 scale_factor と add_offset を解釈する
    * ((<[]>))     NetCDFVar#get と同様だが、サブセットを NArray#[] と同様に指定する. 
    * ((<[]=>))     NetCDFVar#put と同様だが、サブセットを NArray#[]= と同様に指定する.


  "numru/netcdf_miss" を require することで追加されるインスタンスメソッド

    * ((<get_with_miss>))     　((<get>))と同様だが、データ欠損を処理する

    * ((<get_with_miss_and_scaling>))     　((<get_with_miss>))と同様だが、((<unpack>))によりスケーリングも行う.
    * ((<put_with_miss>))     　((<put>))と同様だが、データ欠損を処理する
    * ((<put_with_miss_and_scaling>))     　((<put_with_miss>))と同様だが、((<pack>))によりスケーリングも行う.

* ((<クラス NetCDFAtt>))

  クラスメソッド

  インスタンスメソッド
    * ((<name>))     属性の名前を返す
    * ((<name=>))     属性の名前を変更
    * ((<copy>))     属性を別の変数またはファイルにコピー。ファイルの場合はグローバル属性になる
    * ((<delete>))     属性を削除
    * ((<put>))     属性の値を設定
    * ((<get>))     属性の中身を取り出す
    * ((<atttype>))     属性値の型を問う
    * ((<typecode>))     属性値の型を問う(NArrayのtypecodeで返す)

---------------------------------------------

=クラス NetCDF
===クラスメソッド
---NetCDF.open(filename, mode="r", share=false)
     ファイルオープン（クラスメソッド）mode="w" でファイルが存在しなければ新規作成

     引数
     * filename (String): ファイル名
     * mode (String) : 入出力モード: "r"(読み取りのみ); "w","w+" (書き
       込み -- 現在の中身は上書き(消える!)); "r+","a","a+" (追加 --
       現在の内容はそのままに、書き込み可)。 組み込みの File クラスと
       違い、どのモードでも読み込みは可能。注意：元になる NetCDFライ
       ブラリーの制限により、「追加」には余分な時間とディスクスペース
       がかかる。
     * share (true or false) : sharedモードにするか (書き込み中のファ
       イルに他のプロセスからの読み込みがあり得る場合に true とする。C版ユー
       ザーズガイド第5章の nc_open の項を参照のこと)

     戻り値
     * NetCDFクラスのオブジェクト

     対応する(利用する) C 版 NetCDF の関数
     * nc_open, nc_create

---NetCDF.new
     NetCDF.openメソッドのエイリアスである

---NetCDF.create(filename, noclobber=false, share=false)
     NetCDFファイルを作る（クラスメソッド)
     
     引数
     * filename (String) : ファイル名
     * noclobber (true or false) : 上書きするかしないか
     * share (true or false) : shared mode を使うか (書き込み中のファ
       イルに他のプロセスからの読み込みがあり得る場合に true とする。C版ユー
       ザーズガイド第5章の nc_open の項を参照のこと)

     戻り値
     *  NetCDFクラスのオブジェクト

     対応する(利用する) C 版 NetCDF の関数
     *  nc_create

---NetCDF.create_tmp(tmpdir=ENV['TMPDIR']||ENV['TMP']||ENV['TEMP']||'.', share=false)
     テンポラリーNetCDFファイルを作る（クラスメソッド)
     名前は自動で決まる。クローズされると消される。

     引数
     * tmpdir (String) : テンポラリーファイルを置くディレクトリー名。
       デフォルトは環境変数で指定されたディレクトリー（TMPDIR,TMP,or
       TEMP）または '.'。セキュアーモードでは '.' のみがデフォルトとな
       る。
     * share (true or false) : shared mode を使うか


     戻り値
     *  NetCDFクラスのオブジェクト

     対応する(利用する) C 版 NetCDF の関数
     *  nc_create

===インスタンスメソッド
---close
     ファイルクローズ

     引数
     *  なし

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_close

---ndims
     ファイル中の次元の数を返す

     引数
     *  なし

     戻り値
     *  Integer

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_ndims

---nvars
     ファイル中の変数の数を返す
   
     引数
     *  なし

     戻り値
     *  Integer
     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_nvars

---natts
     ファイル中のグローバル属性の数を返す

     引数
     *  なし

     戻り値
     *  Integer

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_natts

---unlimited
     ファイル中のunlimited dimensionを返す

     引数
     *  なし

     戻り値
     *  存在するときNetCDFDimクラスのオブジェクト。ないときはnil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_unlimdim

---path
     ファイルのパス. open/create時のfilename引数の中身を返す.

     引数
     *  なし

     戻り値
     *  String

     対応する(利用する) C 版 NetCDF の関数
     *  なし

---redef
     define modeにする。既にそうなら何もせずnilを返す。

     引数
     *  なし

     戻り値
     *  true (定義モードへの変更が成功);
	nil (既に定義モード中).
	その他の理由で変更できない場合は例外発生.
     
     対応する(利用する) C 版 NetCDF の関数
     *  nc_redef

---enddef
     data mode に入る。既にそうなら何もせずnilを返す。

     引数
     *  なし

     戻り値
     *  true (データモードへの変更が成功);
	nil (既にデータモード中).
	その他の理由で変更できない場合は例外発生.

     対応する(利用する) C 版 NetCDF の関数
     *  nc_endef

---define_mode?
     今定義モードかどうか問合わせる.

     引数
     *  なし

     戻り値
     *  true (今定義モード);
	false (今データモード);
	nil (その他 -- 読み出し専用など).

     対応する(利用する) C 版 NetCDF の関数
     *  nc_redef と nc_enddef の組み合わせ

---sync
     メモリー中のバッファーをディスク上に反映してファイルを同期させる

     引数
     *  なし

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_sync

---def_dim(dimension_name, length)
     dimensionを定義

     引数
     * dimension_name (String) : 定義するdimensionの名前
     * length (Integer) : dimensionの長さ。無制限次元は 0。

     戻り値
     *  定義された次元 (NetCDFDimオブジェクト)

     対応する(利用する) C 版 NetCDF の関数
     *  nc_def_dim

---put_att(attribute_name, value, atttype=nil)
     グローバル属性を設定

     引数
     * attribute_name (String) : グローバル属性の名前
     * value (Numeric, String, Array of Numeric, or NArray) : 設定する値
     * atttype (nil or String) : 属性の型.
       "char"(="string"),"byte", "sint","int","sfloat", "float" (それ
       ぞれ,1,1,2,4,4,8 バイト)) または nil (つまりお任せ)

     戻り値
     *  設定された属性 (NetCDFAttオブジェクト)

     対応する(利用する) C 版 NetCDF の関数
     *  nc_put_att_<type>

---def_var(variable_name, vartype, dimensions)
     変数を定義

     引数
     * variable_name (String) : 定義するvariableの名前
     * vartype (String or Fixnum) : 変数のタイプ ("char", "byte", "sint", "sint", "int",
       "sfloat", "float" のいずれか、またはNArrayのtypecode(Fixnum))
     * dimensions (Array) : variableの次元。NetCDFDim の Array。最も
       「速く回る」次元から「遅く回る」次元の順に。その長さが変数の
       rank となる。

     戻り値
     *  定義した変数(NetCDFVarオブジェクト)

     対応する(利用する) C 版 NetCDF の関数
     *  nc_def_var

---def_var_with_dim(variable_name, vartype, shape_ul0, dimnames)
     def_varと同じだが必要ならまず次元を定義する。
     既存次元の長さと合わない場合例外。

     引数
     * variable_name (String) : 定義するvariableの名前
     * vartype (String) : 変数のタイプ ("char", "byte", "sint", "sint", "int",
       "sfloat", "float" のいずれか)
     * shape_ul0 (Array of Integer) : 変数の形、即ち、各次元の長さ。無
       制限次元はゼロで表す。長さが変数の rank となる。
     * dimnames (Array of String) : 各次元の名前. 長さ(=>rank) は
       shape_ul0 と等しくなければならない。

     戻り値
     *  定義した変数(NetCDFVarオブジェクト)

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_def_var)

---var(var_name)
     ファイルに既存の変数をオープン

     引数
     * var_name (String) : オープンする変数名

     戻り値
     * NetCDFVar クラスのオブジェクト。存在しなければ nil。

     対応する(利用する) C 版 NetCDF の関数
     * nc_inq_varid

---vars(names = nil)
     ファイル中の変数をまとめてオープン

     引数
     * names (nil or Array of String) : オープンする変数名. nilなら全部（デフォルト）

     戻り値
     * NetCDFVarオブジェクトのArray。namesに存在しない変数名が含まれれば例外発生。

     対応する(利用する) C 版 NetCDF の関数
     * nc_inq_varid

---dim(dimension_name)
     既存の次元をオープン

     引数
     * dimension_name (String) : オープンする次元名

     戻り値
     * NetCDFDimクラスのオブジェクト。存在しなければ nil。

     対応する(利用する) C 版 NetCDF の関数
     * nc_inq_dimid

---dims(names = nil)
     ファイル中の次元をまとめてオープン

     引数
     * names (nil or Array of String) : オープンする次元名. nilなら全部（デフォルト）

     戻り値
     * NetCDFDimオブジェクトのArray。namesに存在しない次元名が含まれれば例外発生。

     対応する(利用する) C 版 NetCDF の関数
     * nc_inq_dimid

---att(attribute_name)
     既存のグローバル属性をオープン

     引数
     * attribute_name (String) : オープンするグローバル属性名

     戻り値
     *  存在すれば NetCDFAttクラスのオブジェクト。存在しなければnil

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_attidを検査に利用)

---fill=(filemode)
     fillmodeの変更。(NetCDF のデフォルトは FILL である。)

     引数
     * fillmode (true for FILL or false for NOFILL)

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_set_fill

---each_dim{ ... }
     次元に関するイテレータ. 
     例: {|i| print i.name,"\n"} で全次元の名前を表示.

     引数
     * { ... } : イテレーター用ブロック。do endブロックでもよい。

     戻り値
     *  self

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_ndimsを利用)

---each_var{ ... }
     変数に関するイテレータ. 
     例: {|i| print i.name,"\n"} で全変数の名前を表示.

     引数
     * { ... } : イテレーター用ブロック。do endブロックでもよい。

     戻り値
     *  self

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_nvarsを利用)

---each_att{ ... }
     グローバル属性に関するイテレータ. 
     例: {|i| print i.name,"\n"} で全属性の名前を表示.

     引数
     * { ... } : イテレーター用ブロック。do endブロックでもよい。

     戻り値
     *  self

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_natts, nc_inq_attnameを利用)

---dim_names
     ファイル中の全次元の名前を配列に入れて返す。

     引数
     *  なし

     戻り値
     *  String の Array

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_ndims, nc_inq_dimnameを利用)

---var_names
     ファイル中の全変数の名前を配列に入れて返す。

     引数
     *  なし

     戻り値
     *  String の Array

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_nvars, nc_inq_varnameを利用)

---att_names
     ファイル中の全グローバル属性の名前を配列に入れて返す。
     引数
     *  なし

     戻り値
     *  String の Array

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_natts, nc_inq_attnameを利用)

---------------------------------------------

=クラス NetCDFDim
===クラスメソッド

===インスタンスメソッド
---length
     次元の長さを返す

     引数
     *  なし

     戻り値
     *  Integer

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_dimlen

---length_ul0
     length と同じだが、無制限次元に関しゼロを返す

     引数
     *  なし

     戻り値
     *  Integer

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_dimlen

---name=(dimension_newname)
     名前をつけかえる

     引数
     * dimension_newname (String) : 新しい名前

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_rename_dim

---name
     名前を返す

     引数
     *  なし

     戻り値
     *  String

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_dimname

---unlimited?
     無制限次元かどうか？

     引数
     *  なし

     戻り値
     *  true or false

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_unlimdim を利用)

---------------------------------------------

=クラス NetCDFVar
===クラスメソッド
---NetCDFVar.new(file,varname,mode="r",share=false)
     NetCDF変数をオープンする。これは、NetCDF#var (NetCDFクラスのイン
     スタンスメソッドvar) を使っても出来るし、そちらのほうを推奨する。

     引数
     * file (NetCDF or String) : NetCDFオブジェクト(NetCDF)
       または NetCDF ファイルのパス(String)
     * varname (String) : file中の変数名
     * mode (String) : 入出力モード。fileが String の時に使われる。
       （NetCDF.openの項を見よ）
     * share (true or false) : sharedモードにするか。
       fileが String の時に使われる。（NetCDF.openの項を見よ）

     戻り値
     * NetCDFVarクラスのオブジェクト

     対応する(利用する) C 版 NetCDF の関数
     * (nc_open, nc_create, nc_inq_varid 等を利用する)

---NetCDFVar.unpack_type=(na_type)
     ((<scaled_get>))で使うNArrayの型を固定する.

     引数
     * na_type (Integer) : NArray::BYTE, NArray::SINT, NArray::INT, 
       NArray::SFLOAT, or NArray::FLOAT

     戻り値
     * na_type (引数)

---NetCDFVar.unpack_type
     ((<NetCDFVar.unpack_type=>))で設定したNArrayの型を返す.

     戻り値
     * nil, NArray::BYTE, NArray::SINT, NArray::INT, 
       NArray::SFLOAT, or NArray::FLOAT

===インスタンスメソッド
---dim(dim_num)
     その変数における dim_num 番目(0から数える)の次元を問合わせる。

     引数
     * dim_num (Fixnum) : 0,1,2,...。最も速く回る次元が0。

     戻り値
     *  NetCDFDimオブジェクト

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_vardimid利用)

---dims
     その変数の全次元を配列にいれて返す

     引数
     *  なし

     戻り値
     *  NetCDFDimオブジェクトのArray

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_vardimid

---shape_ul0
     変数の形を返す. 但し無制限次元の長さはゼロ.
     他の変数の定義に便利.

     引数
     *  なし

     戻り値
     *  Array. [0次元目の長さ, 1次元目の長さ,.. ]

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_vardimid, nc_inq_unlimdim 等を利用)

---shape_current
     変数の現在の形を返す.

     引数
     *  なし

     戻り値
     *  Array. [0次元目の長さ, 1次元目の長さ,.. ]

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_vardimid 等を利用)

---each_att{ ... }
     変数中の全属性(NetCDFAtt)に関するイテレータ

     引数
     * { ... }  : 繰り返すブロック

     戻り値
     *  self

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_natts, nc_inq_attnameを利用)

---dim_names
     変数中の全次元の名前を配列に入れて返す。

     引数
     *  なし

     戻り値
     *  String の Array

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_varndims, nc_inq_vardimid, nc_inq_dimname を利用)

---att_names
     変数中の全属性の名前を配列に入れて返す。

     引数
     *  なし

     戻り値
     *  String の Array

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_natts, nc_inq_attnameを利用)

---name
     変数の名前を返す
 
     引数
     *  なし

     戻り値
     *  String

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_varname

---name=(variable_newname)
     名前を付け替える

     引数
      * variable_newname (String) : 新しい名前

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_rename_var

---ndims
     次元の数を問う

     引数
     *  なし

     戻り値
     *  Integer

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_varndims

---rank
     ndimsのエリアス

---ntype
     vartypeの別名

---vartype
     変数値の型を問う

     引数
     *  なし

     戻り値
     *  String ("char","byte","sint","int","sfloat", or "float")

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_vartype

---typecode
     変数値の型を問う(NArrayのtypecodeで返す)

     引数
     *  なし

     戻り値
     *  Fixnumオブジェクト (NArray:BYTE, NArray:SINT, NArray:LINT, NArray:SFLOAT, NArray:SFLOAT, NArray:DFLOAT)

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_vartype

---natts
     属性の数を問う

     引数
     *  なし

     戻り値
     *  Integer

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_varnatts

---file
     その変数が属するファイルを問合わせる

     引数
     *  なし

     戻り値
     *  NetCDFクラスのオブジェクト

     対応する(利用する) C 版 NetCDF の関数
     *  なし

---att(attribute_name)
     名前を指定した属性を返す

     引数
     * attribute_name (String) : 取り出す属性名

     戻り値
     *  NetCDFAttオブジェクト. 存在しなければnil

     対応する(利用する) C 版 NetCDF の関数
     *  (nc_inq_attidを検査に利用)

---put_att(attribute_name, value, atttype=nil)
     属性を設定

     引数
     * attribute_name (String) : attribute名
     * value (Numeric, String, Array of Numeric, or NArray) : 設定する値
     * atttype (nil or String) : 属性の型.
       "char"(="string"),"byte", "sint","int","sfloat", "float" (それ
       ぞれ,1,1,2,4,4,8 バイト)) または nil (つまりお任せ)

     戻り値
     *  NetCDFAttのオブジェクト

     対応する(利用する) C 版 NetCDF の関数
     *  nc_put_att_<type>

---put(value, option=nil)
     ((<simple_put>))の別名(alias)

---simple_put(value, option=nil)
     値を入れる

     引数
     * value : 入れる値 (Numeric,Array(Numericの1次元配列)、NArray)
       スカラー(Numeric)または長さ１の場合は、その値を一様に設定する。
     * option (Hash) : 変数の一部分を指定するためのオプション引数。省
       略すれば、変数全体が対象となる。ハッシュのキーとしては、
       "start","end","stride"の組、または"index"が使用できる。"index"
       は1要素（スカラー）を指す。"end","stride"は省略可。省略するにし
       ろしないにしろ、書きこむべき要素の数が valueのそれと一致するようにし
       なければならない。start,end,indexではArrayの場合と同様、負の値で後ろ
       からの位置を指定できる。"stride"は正の値のみ受け付けるので、配
       列を引っくり返したければ後から別途やることになる。

       例： 変数が2次元の場合:

       {"start"=>[2,5],"end"=>[6,-1],"stride"=>[2,4]} -- 第1次元目は、
       要素 2 から 6 まで 1 つおき (0から数えるので要素 2 は 3 番目であ
       ることに注意)、第2次元目は、要素 6 から 最後(=-1)まで 3 つおき
       にとったサブセット。

       {"index"=>[0,0]}: 最初の要素にあた
       るスカラー値

       {"index"=>[0,-2]}: 1次元目は最初, 2次元目は最後から2番目にあた
       るスカラー値

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_put_var_<type>, nc_put_vars_<type>, nc_put_var1_<type>

---pack(na)
     selfの属性 scale_factor and/or add_offset を用いて NArray 等を "pack" する.

     もしも scale_factor and/or add_offset が設定されていれば
     (na-add_offset)/scale_factor を返す。そうでなければ na を返す。

     引数
     * na : pack する数値配列 (NArray, NArrayMiss, Array)

     戻り値
     *  a NArray or NArrayMiss

---scaled_put(value, option=nil)
     ((<simple_put>)) と同様だが、((<pack>))により属性 scale_factor と add_offset を解釈する

     引数等については put の解説を参照のこと

---get(option=nil)
     ((<simple_get>))の別名(alias)

---simple_get(option=nil)
     値を取り出す

     引数
     * option (Hash) : 変数の一部分を指定するためのオプション引数。仕
       様は put に同じ。

     戻り値
     *  NArrayクラスのオブジェクト

     対応する(利用する) C 版 NetCDF の関数
     *  nc_get_var_<type>, nc_get_vars_<type>, nc_get_var1_<type>

---unpack(na)
     selfの属性 scale_factor and/or add_offset を用いて NArray 等を "unpack" する.

     もしも scale_factor and/or add_offset が設定されていれば
     na * scale_factor + add_offset を返す。そうでなければ na を返す。
     coerce によって型が変化する -- 例えば、もし na が sint で
     scale_factor と add_offset が sfloat であれば、返り値は sfloat
     になる。返り値の型は ((<NetCDFVar.unpack_type=>)) を使って
     陽に指定することもできる。

     引数
     * na : unpack する数値配列 (NArray, or NArrayMiss)

     戻り値
     *  a NArray or NArrayMiss


---scaled_get(option=nil)
     ((<simple_get>)) と同様だが、((<unpack>))により属性 scale_factor と add_offset を解釈する

     引数等については put の解説を参照のこと

---[]
     NetCDFVar#get と同様だが、サブセットを NArray#[] と同様に指定する. 

     NArrayでサポートされているサブセット指定方法に加えて、ステップ付
     きの範囲を指定できる。これは {0..-1,2} などとする。つまり、要素
     数が１のハッシュのキーに範囲(Range)、値にステップ(Integer)を指定
     する。NArrayと違い、２次元以上の変数を１次元のインデックスで
     指定することは出来ない。

---[]=
     NetCDFVar#put と同様だが、サブセットを NArray#[]= と同様に指定する. 

     NArrayでサポートされているサブセット指定方法に加えて、ステップ付
     きの範囲を指定できる。これは {0..-1,2} などとする。つまり、要素
     数が１のハッシュのキーに範囲(Range)、値にステップ(Integer)を指定
     する。NArrayと違い、２次元以上の変数を１次元のインデックスで
     指定することは出来ない。

==="numru/netcdf_miss" を require することで追加されるインスタンスメソッド

---get_with_miss(option=nil)
     ((<get>))と同様だが、データ欠損を処理する

     データ欠損は標準属性 valid_range, (valid_min and/or valid_max), 
     または missing_value により指定される。解釈の優先順位はこの順番で
     ある。「ユーザーズガイド」の推奨と違い、ここでは missin_value も
     解釈される。もし missing_value と valid_* が同時に存在する場合、
     missng_value は有効データ範囲外にあってはならない。その場合、例外
     が発生する。

     上記のようにもしデータ欠損（の仕方）が指定されていれば、このメソッ
     ドは NArrayMiss オブジェクトを返す。そうでなければ NArray を返す。

     引数
     * ((<get>))を見よ.

     戻り値
     * NArrayMissオブジェクト (データ欠損が指定さている場合) または NArray
       オブジェクト (データ欠損が指定さてない場合)

     NetcdfError 以外の例外発生
     * missing_value がデータ有効範囲にない (上記参照のこと).

     対応する(利用する) C 版 NetCDF の関数
     * ((<get>)) を見よ. このメソッドは Ruby のみで書かれている. 

     例
     * 次の例では ((<get>)) を ((<get_with_miss>)) を置き換えている.
       こうすると ((<[]>)) もデータ欠損を解釈するようになる (内部で 
       (({get})) を呼んでるので).

         file = NetCDF.open('hogehoge.nc')
         var = file.var('var')
         def var.get(*args); get_with_miss(*args); end
         p var.get       # --> inteprets data missing if defined
         p var[0..-1,0]  # --> inteprets data missing if defined (assumed 2D)


---get_with_miss_and_scaling(option=nil)
     ((<get_with_miss>))と同様だが、((<unpack>))によりスケーリングも行う.

     欠損値処理は基本的には pack したデータを対象に行う(これは多くの
     コンベンション採用されている)。ただし、valid_* / missing_value
     の型が pack したデータの型と異なり、かつ unpackしたデータの型と
     一致するとき（に限り） unpack したデータを対象とする。
     これで基本的に全てのコンベンションに対応できる。

     例
     * 上記参照。同じことができる。

---put_with_miss(value, option=nil)
     ((<put>))と同様だが、データ欠損を処理する

     もしも (({value})) が NArray なら、このメソッドは ((<put>)) に同
     じ. もし(({value})) が NArrayMiss でかつ self の属性によりデータ欠損が
     定義されていれば (((<get_with_miss>))を参照のこと)、(({value})) 
     中のデータ欠損が解釈される。即ち、(({value}))中の欠損データは、
     ある欠損値 (missing_value または _FillValue またはデータ有効範囲
     外の適当な値) に置き換えられた上でファイルに書き込まれる。
     (({value})) 中の非欠損データが (({self})) における有効範囲に入って
     いるかどうかはチェックされない。

     引数
     * value : NArrayMiss または ((<put>)) で許されているクラスのオブジェクト.
     * option (Hash) : ((<put>)) を見よ.

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     * ((<put>)) を見よ. このメソッドは Ruby のみで書かれている. 

     例
     * 次の例では ((<put>)) を ((<put_with_miss>)) を置き換えている.
       こうすると ((<[]=>)) もデータ欠損を解釈するようになる (内部で 
       (({put})) を呼んでるので).

         file = NetCDF.open('hogehoge.nc')
         var = file.var('var')
         def var.put(*args); put_with_miss(*args); end
         var.put = narray_miss      # --> inteprets data missing if defined
         var[0..-1,0] = narray_miss # --> inteprets data missing if defined (assumed 2D)

---put_with_miss_and_scaling(value, option=nil)
     ((<put_with_miss>))と同様だが、((<pack>))によりスケーリングも行う.

     欠損値処理は基本的には pack したデータを対象に行う(これは多くの
     コンベンション採用されている)。ただし、valid_* / missing_value
     の型が pack したデータの型と異なり、かつ unpackしたデータの型と
     一致するとき（に限り） unpack したデータを対象とする。
     これで基本的に全てのコンベンションに対応できる。

     例
     * 上記参照。同じことができる。

---------------------------------------------

=クラス NetCDFAtt
===クラスメソッド

===インスタンスメソッド
---name
     属性の名前を返す

     引数
     *  なし

     戻り値
     *  String

     対応する(利用する) C 版 NetCDF の関数
     *  なし

---name=(attribute_newname)
     属性の名前を変更

     引数
     * attribute_newname (String) : 新しい名前

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_rename_att

---copy(var_or_file)
     属性を別の変数またはファイルにコピー。ファイルの場合はグローバル属性になる

     引数
     * var_or_file (NetCDFVar or NetCDF)

     戻り値
     *  コピーして作られた属性 (NetCDFAtt)

     対応する(利用する) C 版 NetCDF の関数
     *  nc_copy_att

---delete
     属性を削除

     引数
     *  なし

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_del_att

---put(value, atttype=nil)
     属性の値を設定

     引数
     * value (Numeric, String, Array of Numeric, or NArray) : 入れる値
     * atttype (nil or String) : 属性の型.
       "char"(="string"),"byte", "sint","int","sfloat", "float" (それ
       ぞれ,1,1,2,4,4,8 バイト)) または nil (つまりお任せ)

     戻り値
     *  nil

     対応する(利用する) C 版 NetCDF の関数
     *  nc_put_att_<type>

---get
     属性の中身を取り出す

     引数
     *  なし

     戻り値
     *  文字列またはNArrayオブジェクト (注意: スカラーの場合も長さ1の
	NArrayになる)
     
     対応する(利用する) C 版 NetCDF の関数
     *  nc_get_att_<type>

---atttype
     属性値の型を問う

     引数
     *  なし

     戻り値
     *  "char","byte","sint","int","sfloat","float"

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_atttype

---typecode
     属性値の型を問う(NArrayのtypecodeで返す)

     引数
     *  なし

     戻り値
     *  Fixnumオブジェクト (NArray:BYTE, NArray:SINT, NArray:LINT, NArray:SFLOAT, NArray:SFLOAT, NArray:DFLOAT)

     対応する(利用する) C 版 NetCDF の関数
     *  nc_inq_atttype

=end
