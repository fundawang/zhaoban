function DBC2SBC(str) { 
	var i; 
	var result=''; 
	for(i=0;i<str.length;i++) { 
		code=str.charCodeAt(i); 
		if(code>=65281&&code<65373)    
			result+=String.fromCharCode(str.charCodeAt(i)-65248); 
		else result+=str.charAt(i);
	} 
	return result; 
}

jQuery.cookie = function(name, value, options) {
    if (typeof value != 'undefined') { // name and value given, set cookie
        options = options || {};
        if (value === null) {
            value = '';
            options.expires = -1;
        }
        var expires = '';
        if (options.expires && (typeof options.expires == 'number' || options.expires.toUTCString)) {
            var date;
            if (typeof options.expires == 'number') {
                date = new Date();
                date.setTime(date.getTime() + (options.expires * 24 * 60 * 60 * 1000));
            } else {
                date = options.expires;
            }
            expires = '; expires=' + date.toUTCString(); // use expires attribute, max-age is not supported by IE
        }
        var path = options.path ? '; path=' + options.path : '';
        var domain = options.domain ? '; domain=' + options.domain : '';
        var secure = options.secure ? '; secure' : '';
        document.cookie = [name, '=', encodeURIComponent(value), expires, path, domain, secure].join('');
    } else { // only name given, get cookie
        var cookieValue = null;
        if (document.cookie && document.cookie != '') {
            var cookies = document.cookie.split(';');
            for (var i = 0; i < cookies.length; i++) {
                var cookie = jQuery.trim(cookies[i]);
                // Does this cookie string begin with the name we want?
                if (cookie.substring(0, name.length + 1) == (name + '=')) {
                    cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                    break;
                }
            }
        }
        return cookieValue;
    }
};

function deal_input_palceholder() {
jQuery(function($){
    if (!$.support.placeholder) {
        $('input[placeholder], textarea[placeholder]').each(function(i, el){
            el = $(el);
            var defValue = el.attr('placeholder'), defColor = el.css('color');
            el.focus(function(){
                if (this.value === '' || this.value === defValue) {
                    $(this).css('color', defColor);
                    this.value = '';
                }
            })
            .blur(blurHandler)
            .closest('form').submit(function(){
                if (el.val() === defValue) {
                    el.val('');
                }
            });
            blurHandler.call(el[0]);
            function blurHandler(){
                if (this.value === '' || this.value === defValue) {
                    $(this).css('color', '#999');
                    this.value = defValue;
                }
            }
        });
    }
});}
