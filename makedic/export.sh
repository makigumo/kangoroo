echo "charset utf8;select japanese,reading,german from entries where status=0 and delstatus=0;" | mysql -u root wadoku > wadoku.utf8