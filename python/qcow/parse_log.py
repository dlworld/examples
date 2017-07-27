import json

def parse_read_sectors(log_file):
    stats = {}
    min_sector = 0x0fffffff
    max_sector = 0
    sectors = []
    with open(log_file) as f:
        for line in f.readlines():
            line = line.strip()
            if not line or not line.startswith('0x'):
                continue

            sector, cluster_offset, length, _ = line.split(' ')
            if stats.get(cluster_offset):
                stats[cluster_offset]['sectors'].append(sector)
                stats[cluster_offset]['count'] += 1
            else:
                stats[cluster_offset] = {'sectors':[sector], 'count':1}

            sector = int(sector, 16)
            if sector < min_sector:
                min_sector = sector

            if sector > max_sector:
                max_sector = sector

            if sector not in sectors:
                sectors.append(sector)

    stats['min_sector'] = min_sector
    stats['max_sector'] = max_sector
    sectors.sort()
    stats['sectors'] = sectors

    return stats

if __name__ == '__main__':
    stats = parse_read_sectors('/root/win7_read_sectors.log')
    #print json.dumps(stats, sort_keys=True, indent=4, separators=(',', ': '))
    #print stats.keys() 
    #print len(stats.keys())
    print stats['sectors']

