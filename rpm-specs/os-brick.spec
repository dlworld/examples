
Name:             python2-os-brick
Version:          0.5.0
Release:          11
Summary:          python2-os-brick

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          os-brick-%{version}.tar.gz

BuildArch:        noarch

%description
python2 os brick

%prep
%setup -q -n os-brick-%{version}
%autosetup -n os-brick-%{version} -S git

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}
mkdir %{buildroot}/etc
mkdir -p %{buildroot}/usr/share/os-brick/rootwrap
cp -a etc/os-brick %{buildroot}/etc/
cp etc/os-brick/rootwrap.d/* %{buildroot}/usr/share/os-brick/rootwrap/
compile_config="/root/os-brick.conf"
python /root/deletepy.py %{buildroot} ${compile_config}


%pre

%post

%preun

%postun

%files
%{python2_sitelib}/os_brick
%{python2_sitelib}/os_brick-*.egg-info
/usr/etc/os-brick/rootwrap.d/os-brick.filters
/usr/share/os-brick
/etc/os-brick

%changelog
