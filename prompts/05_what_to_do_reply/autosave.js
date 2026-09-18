(() => {
  const el = document.querySelector(<SEL>);
  if (!el) return null;
  el.scrollIntoView({block:'center', inline:'center'});
  const r = el.getBoundingClientRect();
  if (r.width <= 0 || r.height <= 0) return null;
  return { x: r.left + r.width/2, y: r.top + r.height/2, width: r.width, height: r.height, tagName: el.tagName.toLowerCase() };
})()
