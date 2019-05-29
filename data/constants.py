#!/usr/bin/python3
import os
import re


n =[0xc26e8d2105e3454baf122700611e915d, 
	0x5f4c6991b0042610e525a5c4981737a1, 
	0xc8aed04da6c85dd4638add6c6fc04a59, 
	0x8ae8fe509879989bb1314c5a1ac92506a95a284a336cfe29d9a60b5113eeb33b,
	0x34350b132f9ec752124e3763a04b39e7320565f3b79aa947f114e510d9e166a1,
	0xf16ea639730a5881f346ac3c065d11322b3be9c7def104269d88ed0c73bba1b05c84712bca9f0880916cab52ea3a88f04cbc2d45ee00647293b1a7c43e1acc3d,
	0xb8db0a56ef03b60c5171980681d05dd81a297500e9b06ad3f3b05bb2e1985e8de10f8eeccd0d962bf993c383092d510a5e44e43e4b2dd74ac3295f2fdc6204f1,
	0x70b7ad9ba748fff86b9b8bfd20b97afe06f9eed59e34c8fd40b5cf614c2201687cbe6ce266502682b723eae67220c440f98db2967c7db53bc1095d035636232766b2e16e26eb94caf7af56286d82cec2824b1509591d60ed08e9b2db50cec84cbcab83b191878628c4daea65d21051ce916f9941d603a96b349c83ae120ee3b5
	0x63eee9ff13ee48d66c85787113622e5933ae4f2e8bdff62549a94800c21ba4f3b4406c06bc2b63ac706b4a2e47b783102fac11e119be2b08ce0aad36246c4d8ed5de93e701f1cb4c6e035f3dd0284e737a5c758bf8606508f19c16cf37f17bc1cfad7dbaff50f0e6e30236b7fcf54478da35def9f2311bc50387ed6236e7adff
	0x7a13785f56f48c54d12466cf178f96d3c8b7ed526b1e632874d877eea7c6e8cdc998d166bf923e24f941330adca4b281c0cdee1e82d5a54c3da8fa3325c902a94ef978ea5ebb1e693e4451bb1acd8054f7921d66c0be60abc99d7dee9e8c3e56acc17df5a8dd9ecc5588a7554fe9d0782eeda3042bee1b1321589fa0e48527dc0a80b56011c02dd5e05227e4be96a5c46a88819732f236ed50a9a792d5e8817e658bc86b988066d1031b82d2612e20a94dc8e97900eda4e153b1a2e6b2b0a05fb6a967880fde47743a12dc7aa652e195d6938310a2f9163a365081aae6e4c8acd2a64c2c60dc7e63c106de551846e7be16515c3ff99d4f956651d968b078c7e1
	0x9f4fa78ebad8a50f508199722031859a2cf8bc232124712fc6eafb54e84dec7cdd378f6a4fcaff333c928382157fbb4935c53241f207ff0fd0944dffe8b2cdfece689b43af7570806cbbf1a594770b3d24739f44530a6015c91faa9e305bc0a3de9ae4105bc0abec0245d4b6de232326bb9704f5cba348e5439fb1db31868fe760941d11aa42ec80eb6ee7345893678b9b1ac32f142f42d60f983b1d0b29adf5abaf84125e238992c8036af7e341f31a9425eddfcf8a42e835b4e6edd974106f8790f7dbc9c8453cde55b51e794928975c5258af49ec06c154598f3d811626414d672ea98880d91f7dbf1dea0e28aeee15ab67674c2ef6b97510712a0a63025b
	]

keys = [0x0745812bb1ffacf0b5d6200be2ced7d5, 
		0x4c0929ca2608895dc42fbc87ef853e09, 
		0x2845ecc7a890cd4356ef00ff86e63f81, 
		0x1d8232d4f22da2e0705cac6d27a4d839149c23bd63746e9957bef4e85ae8f2b7;
		0xbde5d7cd907ff14d2745b26d7f83ffa00254b33822c4d131724ae2dd4b7e8663;
		0x05ffea52c644ee4e589eef5d6ddaf173eeee2c1ee520e88af79b3e974c5ef1d5814ea45a531a7afa872f02990ced7ef59709e4aff18f0c6ed18724cb8e505ef1,
		0x191858e17018a85a8de29403596a33b53792f691026d0dc2f21661ff48f558f0693fa9b206eb139a7cb8f5df1a33d1c7227dd94b837fe64cf3a977d1ec5c5cd1,
		0x9e1770c77f71106d352173ad61a54b8db2884e29fa19b2094fe53c2ad2ccfa4b027068ac9b0740535a3dbb08c9bb7e8cf67d45b44f433ee5d412282c8df46f27f8b7963e1ba718b8cf4b6436e00cd15e9723a1ddb7b57a707be972b7410d14ac6ee727e593036aff4783c0e708e1f49fdc288a803f0cc0d244924ba12cbbee1b,
		0xfd149e15687d6430ef21f0c083619c2baa6fa6435e3b83a7b00238e752d610d83b7d6459e6caaa138577df7b4dfe1cbe6001df3862f89a2454c1b5925c18884ddb0a5e0e71e92fea90aba974e58914a153578efcd36999ec069a03724b7df922a69218eca4ed02c5b49d05c3ef71641418857b8d9ca724799d6109ae1b0c3a13,
		0xad669106fc7885226094033a92b6eb3254329bdf67c49faae96d3dd1977974e62caaea5b733cede828599e64d5da60d255ecf965d801ae36644fad1a09589116abc4719ca96a265acfb8b9fd6b57ae0a4a9bfb7c62526400851ce2c6c6644388b5a16528e500d69447bb297914098761446424f01d33c837300da1b1513d0d0e721d33f8f316f6f79741b28c077cc23de9757ccafbb47010ccf81b72697d3566dccc257297ac5084387345142c19e1f3d58f87e8b66b7003eadb103de1c6848937260177bf8d2d074e76e1c8576b29b81c1106cb7d6a29c0a219569e7735f6abfbba68c61a55c1928ba746f68e013936108a23692e7ea7600ca88975a3e57d27,
		0xc1390f382355b6e015c83e6eee0988851a7dcac8124fcbf78935030ba8abde34eae77c20888b4d8213f8707cb178385a1f550b6e1174fe2c04b848a00bd4ddd7c38922cb4e98c6657d76742e9ea475ea849e256eee3d7f9cd7562a6736f14d234b2cc0c9cbcb39fa7c8ce8ead278d2dd2cde78dee7f2fa16e9022f62f83861673e07c08a4973b61f0fb09dde49dd20d9cf05ec4f498debf1e63c7bb3b316eb45e3c930238839ba861acd6fa39b6d7535075df1ef853f640cffb235d78a1e97a36899c19a436cd639a2a19539ecb8f4627acc4bb1789d81c817ac49d900a1fdab52504a60f890fd2bd7e2d325e63a6fa9c82c94365766ed9982271f23838a885d
		]

k0=[0x8354f24c98cfac7a6ec8719a1b11ba4f, 
	0x0424eafd3cea52419284de3f6da92934, 
	0x64d8149d5c75b7137c099ce764ab8335, 
	0x2792adec387c47a694860bac38ca8ed7fb28e4c04f74695f7f35a915e3ed6b07,
	0x34350b132f9ec752124e3763a04b39e7320565f3b79aa947f114e510d9e166a1,
	0x3160493be94f6b0dac60adf43f295163782d4a53c9e811b6ccd10bb12c62f57cc3ff9aee5272bb775cf4aa1626ee023a1f90c86e3ffb3baa4ca499fd3e6e6cea,
	0x30bed69e48d03d9ef171d7925d216d4097aa79e7ad72bc61afb1a94df9eb5ca35d51e52f2528bb9f1d123887bdefa2cf63e8b38c8a711239114468e4f381389b,
	0x70b7ad9ba748fff86b9b8bfd20b97afe06f9eed59e34c8fd40b5cf614c2201687cbe6ce266502682b723eae67220c440f98db2967c7db53bc1095d035636232766b2e16e26eb94caf7af56286d82cec2824b1509591d60ed08e9b2db50cec84cbcab83b191878628c4daea65d21051ce916f9941d603a96b349c83ae120ee3b5,
	0x63eee9ff13ee48d66c85787113622e5933ae4f2e8bdff62549a94800c21ba4f3b4406c06bc2b63ac706b4a2e47b783102fac11e119be2b08ce0aad36246c4d8ed5de93e701f1cb4c6e035f3dd0284e737a5c758bf8606508f19c16cf37f17bc1cfad7dbaff50f0e6e30236b7fcf54478da35def9f2311bc50387ed6236e7adff,
	0x7a13785f56f48c54d12466cf178f96d3c8b7ed526b1e632874d877eea7c6e8cdc998d166bf923e24f941330adca4b281c0cdee1e82d5a54c3da8fa3325c902a94ef978ea5ebb1e693e4451bb1acd8054f7921d66c0be60abc99d7dee9e8c3e56acc17df5a8dd9ecc5588a7554fe9d0782eeda3042bee1b1321589fa0e48527dc0a80b56011c02dd5e05227e4be96a5c46a88819732f236ed50a9a792d5e8817e658bc86b988066d1031b82d2612e20a94dc8e97900eda4e153b1a2e6b2b0a05fb6a967880fde47743a12dc7aa652e195d6938310a2f9163a365081aae6e4c8acd2a64c2c60dc7e63c106de551846e7be16515c3ff99d4f956651d968b078c7e1,
	0x9f4fa78ebad8a50f508199722031859a2cf8bc232124712fc6eafb54e84dec7cdd378f6a4fcaff333c928382157fbb4935c53241f207ff0fd0944dffe8b2cdfece689b43af7570806cbbf1a594770b3d24739f44530a6015c91faa9e305bc0a3de9ae4105bc0abec0245d4b6de232326bb9704f5cba348e5439fb1db31868fe760941d11aa42ec80eb6ee7345893678b9b1ac32f142f42d60f983b1d0b29adf5abaf84125e238992c8036af7e341f31a9425eddfcf8a42e835b4e6edd974106f8790f7dbc9c8453cde55b51e794928975c5258af49ec06c154598f3d811626414d672ea98880d91f7dbf1dea0e28aeee15ab67674c2ef6b97510712a0a63025b
	]

vi = [0x2f371a9a1fa7559343df18cc8871c138]
vf = [0x0bc73062f609e7c1e055a2f47df2a69e]

vi_M = [0x02bbe7e3a87bff652d8c132e4179d9f8]
vf_M = [0x987641e5532d7be197729a00e4aa88f4]


bits = [128, 128, 128, 256, 256, 512, 512, 1024, 1024, 2048, 2048]

class Data(object):
	"""docstring for Data"""
	def __init__(self, key, k0, n, time, message):
		super(Data, self).__init__()
		self.private_key = key
		self.k0 = k0
		self.modulus = n
		self.msg_file = message
		self.time_file = time
		self.length = os.stat(time).st_size//8
		self.nb = int(time[-7:-4])


time_file = [f for f in os.listdir('.') if f[0] == 'T' and f[-3:] == 'BIN']
msg_file = [f for f in os.listdir('.') if f[0] == 'P' and f[-3:] == 'BIN']

time_file.sort()
msg_file.sort()

data_l = []
for ms,t in zip(msg_file,time_file):
	index = int(re.search('key(?P<index>[0-9]+)',t).group('index'),10)
	data_l.append(Data(keys[index],k0[index],n[index],t,ms))


import random
import gmpy2

def get_blinding_pair(key,n,nb):
	vi = random.getrandbits(nb)
	while gmpy2.gcd(vi,n) != 1:
		vi = random.getrandbits(nb)
	t = pow(vi,k,n)
	vf = gmpy2.invert(t,n)
	vi_M = vi * 2**(nb+2) % n
	vf_M = vf * 2**(nb+2) % n
	return vi_M, vf_M, vi, vf

