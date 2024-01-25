#!/usr/bin/python
'''Add and remove names in the supporters list.

Usage:
  python3 donors.py -a "New Name"
  python3 donors.py -d "Name To Remove"
  python3 donors.py -i file_of_names_to_add.txt

The appendix-donor.tex file is updated in place. Use git revert to undo
changes.
'''

import argparse
import re
import sys

DEFAULT_DONOR_FILE = 'appendix-donors.tex'
PAGE_START = '''\\setlength{\\tabcolsep}{1mm}
\\begin{tabular}{p{4cm}p{4cm}p{4cm}}
'''
PAGE_NEWPAGE = '''\\newpage
'''
PAGE_END = '''\\end{tabular}
'''
PAGE_LINE_COUNT = 43
PAGE_LINE_COUNT_NEXT = 46
NAME_LINE_RE = re.compile(r'(.*) & (.*) & (.*)\s*\\\\')


def list_get(lst, idx):
    try:
        return lst[idx]
    except IndexError:
        return None


def norm_letters(name):
    for a, b in (
            ('é', 'e'),
            ('ş', 's'),
            ('ü', 'u'),
            ('ö', 'o'),
            ('ä', 'a')):
        name = name.replace(a, b)
    return name


class Donors:
    def __init__(self):
        self.preamble = None
        self.postamble = None
        self.names = []

    @classmethod
    def from_donor_str(cls, txt):
        donors = cls()
        pos = 0
        while True:
            page_start_i = txt.find(PAGE_START, pos)
            if page_start_i == -1:
                donors.postamble = txt[pos:]
                break
            if donors.preamble is None:
                donors.preamble = txt[:page_start_i]
            page_end_i = txt.find(PAGE_END, page_start_i)
            if page_end_i == -1:
                raise ValueError(
                    'Found a page start without a page end, pos ' +
                    str(page_start_i))
            pos = page_end_i + len(PAGE_END)
            page_lines = txt[page_start_i:page_end_i].split('\n')
            for line in page_lines:
                m = NAME_LINE_RE.match(line)
                if not m:
                    continue
                donors.names.extend(
                    [n.strip() for n in m.groups() if n.strip()])
        return donors

    @classmethod
    def from_donor_file(cls, path=DEFAULT_DONOR_FILE):
        with open(path) as fh:
            return cls.from_donor_str(fh.read())

    def to_donor_str(self):
        self.names.sort(key=lambda x: norm_letters(x.strip().lower()))
        parts = []
        parts.append(self.preamble or '')
        pagenum = 1
        name_i = 0
        line_count = PAGE_LINE_COUNT
        while name_i < len(self.names):
            cols = [self.names[name_i:name_i + line_count],
                    self.names[name_i + line_count:name_i + line_count*2],
                    self.names[name_i + line_count*2:name_i + line_count*3]]
            if pagenum > 1:
                parts.append('% page ' + str(pagenum) + '\n')
                parts.append(PAGE_NEWPAGE)
            parts.append(PAGE_START)
            for n in range(line_count):
                parts.append(list_get(cols[0], n) or '')
                parts.append(' & ')
                parts.append(list_get(cols[1], n) or '')
                parts.append(' & ')
                parts.append(list_get(cols[2], n) or '')
                parts.append(' \\\\\n')
            parts.append(PAGE_END)
            name_i += line_count * 3
            pagenum += 1
            line_count = PAGE_LINE_COUNT_NEXT
        parts.append(self.postamble or '')
        return ''.join(parts)

    def to_donor_file(self, path=DEFAULT_DONOR_FILE):
        with open(path, 'w') as fh:
            fh.write(self.to_donor_str())

    def add_name(self, name):
        self.names.append(name.strip())

    def delete_name(self, name):
        name = name.strip()
        name_i = self.names.find(name)
        if name_i != -1:
            del self.names[name_i]


def main(raw_args):
    arg_parser = argparse.ArgumentParser(
        prog='donors',
        description='Add and delete names from the donors list appendix')
    arg_parser.add_argument('-a', '--add', action='append')
    arg_parser.add_argument('-d', '--delete', action='append')
    arg_parser.add_argument('-i', '--input-file')
    arg_parser.add_argument('--donor-file', default=DEFAULT_DONOR_FILE)

    args = arg_parser.parse_args(raw_args)

    dnrs = Donors.from_donor_file(args.donor_file)
    for name in args.add:
        dnrs.add_name(name)
    for name in args.delete:
        dnrs.delete_name(name)
    dnrs.to_donor_file()


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
