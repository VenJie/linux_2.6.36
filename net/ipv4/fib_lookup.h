#ifndef _FIB_LOOKUP_H
#define _FIB_LOOKUP_H

#include <linux/types.h>
#include <linux/list.h>
#include <net/ip_fib.h>

struct fib_alias {					/*[net/ipv4/fib_lookup.h]*/
	struct list_head	fa_list;	/*将所有fib_alias组成链表*/
	struct fib_info		*fa_info;	/*指向fib_info，储存如何处理消息的路由信息*/
	u8			fa_tos;				/*路由的服务类型比特位字段*/
	u8			fa_type;			/*路由表项的类型，间接定义了当路由查找匹配时，应采取的动作*/
	u8			fa_scope;			/*路由表项的作用范围*/
	u8			fa_state;			/*一些标志位，目前只有FA_S_ACCESSED。表示该表项已经被访问过*/
#ifdef CONFIG_IP_FIB_TRIE
	struct rcu_head		rcu;
#endif
};

#define FA_S_ACCESSED	0x01

/* Exported by fib_semantics.c */
extern int fib_semantic_match(struct list_head *head,
			      const struct flowi *flp,
			      struct fib_result *res, int prefixlen);
extern void fib_release_info(struct fib_info *);
extern struct fib_info *fib_create_info(struct fib_config *cfg);
extern int fib_nh_match(struct fib_config *cfg, struct fib_info *fi);
extern int fib_dump_info(struct sk_buff *skb, u32 pid, u32 seq, int event,
			 u32 tb_id, u8 type, u8 scope, __be32 dst,
			 int dst_len, u8 tos, struct fib_info *fi,
			 unsigned int);
extern void rtmsg_fib(int event, __be32 key, struct fib_alias *fa,
		      int dst_len, u32 tb_id, struct nl_info *info,
		      unsigned int nlm_flags);
extern struct fib_alias *fib_find_alias(struct list_head *fah,
					u8 tos, u32 prio);
extern int fib_detect_death(struct fib_info *fi, int order,
			    struct fib_info **last_resort,
			    int *last_idx, int dflt);

static inline void fib_result_assign(struct fib_result *res,
				     struct fib_info *fi)
{
	if (res->fi != NULL)
		fib_info_put(res->fi);
	res->fi = fi;
	if (fi != NULL)
		atomic_inc(&fi->fib_clntref);
}

#endif /* _FIB_LOOKUP_H */
